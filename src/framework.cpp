#include "framework.h"

namespace AC_DataConcentrator
{
	Framework::Framework()
	{

	}

	bool Framework::Initialise()
	{
		/* Local variables */
		bool result = false;

		/* Flow variables */
		unsigned int i = 0;


		/* Process the data directory to build data set */
		result = ProcessDataDirectory("../data/network");
		if (result == false)
		{
			return result;
		}

		/* Initialise networking */
		printf("Initialising Winsock\n");
		if (WSAStartup(MAKEWORD(2, 2), &_wsa) != 0)
		{
			return false;
		}

		/* Initialise each input */
		for (std::vector<SensorInput*>::iterator it = _inputs.begin(); it != _inputs.end(); ++it)
		{
			SensorInput* inputSensor = (SensorInput*)*it;

			/* Print out input information */
			printf("Sensor: %s on port %d\n", inputSensor->name, inputSensor->sourcePort);
		
			/* Open Sockets for each input */
			inputSensor->socket = socket(AF_INET, SOCK_DGRAM, 0);
			if (inputSensor->socket == SOCKET_ERROR)
			{
				printf("ERROR: Unable to open Socket\n");
				return false;
			}

			/* Setup address structure */
			memset((char *)&inputSensor->socketAddr, 0, sizeof(inputSensor->socketAddr));
			inputSensor->socketAddr.sin_family = AF_INET;
			inputSensor->socketAddr.sin_port = htons(inputSensor->sourcePort);
			inputSensor->socketAddr.sin_addr.S_un.S_addr = INADDR_ANY;
						
			/* Bind IP Port */
			if (bind(inputSensor->socket, (struct sockaddr *)&inputSensor->socketAddr, sizeof(inputSensor->socketAddr)) == SOCKET_ERROR)
			{
				printf("ERROR: Bind failed with error code : %d", WSAGetLastError());
				return false;
			}

			/* Create Threads for each sensor */
			std::thread* thread = new std::thread(&Framework::InputSensorFunction, this, inputSensor);
		}

		/* Initialise each output */
		for (std::vector<NetworkOutput*>::iterator it = _outputs.begin(); it != _outputs.end(); ++it)
		{
			NetworkOutput* networkOutput = (NetworkOutput*)*it;

			/* Allocate Message buffers */
			networkOutput->outputStructure.AllocateBuffer();

			/* Bind the Sensor values to the output structure */
			for (i = 0; i < networkOutput->outputStructure.blockCount; i++)
			{
				for (std::vector<SensorInput*>::iterator inputIt = _inputs.begin(); inputIt != _inputs.end(); ++inputIt)
				{
					SensorInput* sensorInput = (SensorInput*)*inputIt;

					if (strcmp(networkOutput->outputStructure.blocks[i].name, sensorInput->name) == 0)
					{
						networkOutput->outputStructure.blocks[i].boundValue = &sensorInput->currentValue;
					}
				}
			}

			/* Open Sockets for each Network output */
			networkOutput->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (networkOutput->socket == SOCKET_ERROR)
			{
				printf("ERROR: Unable to open Socket\n");
				return false;
			}

			/* Setup address structure */
			memset((char *)&networkOutput->socketAddr, 0, sizeof(networkOutput->socketAddr));
			networkOutput->socketAddr.sin_family = AF_INET;
			networkOutput->socketAddr.sin_port = htons(networkOutput->destinationPort);
			networkOutput->socketAddr.sin_addr.S_un.S_addr = inet_addr(networkOutput->destinationIPAddress);

		}

		return result;
	}

	bool Framework::Frame()
	{
		/* Flow variables */
		unsigned int i = 0;

		/* Networking variables */
		int slen = 0;				/* socket address length */
	
		for (std::vector<NetworkOutput*>::iterator it = _outputs.begin(); it != _outputs.end(); ++it)
		{
			NetworkOutput* networkOutput = (NetworkOutput*)*it;

			slen = sizeof(networkOutput->socketAddr);	/* socket address length */
		
			/* Construct Message */
			networkOutput->outputStructure.BuildBuffer();

			/* Send over the network */
			if (sendto(networkOutput->socket, networkOutput->outputStructure.msgBuffer, networkOutput->outputStructure.totalSize, 0, (struct sockaddr *) &networkOutput->socketAddr, slen) == SOCKET_ERROR)
			{
				printf("ERROR: sendto() failed with error code : %d", WSAGetLastError());
				return false;
			}
		}
		return true;
	}

	void Framework::InputSensorFunction(SensorInput* sensor)
	{
		bool running = true;
		int recievedLength = 0;

		struct timeval read_timeout;
		read_timeout.tv_sec = 0;
		read_timeout.tv_usec = 1;

		/* Set the time for the receive function */
		setsockopt(sensor->socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&read_timeout, sizeof(read_timeout));

		while (running)
		{
			/* Clear the buffer by filling null, it might have previously received data */
			memset(sensor->buf, '\0', 255);

			/* Reset the recieved length */
			recievedLength = sizeof(sensor->socketOtherAddr);

			/* try to receive some data */
			if ((recievedLength = recvfrom(sensor->socket, sensor->buf, 255, 0, (struct sockaddr *) &sensor->socketOtherAddr, &recievedLength)) == SOCKET_ERROR)
			{
				printf("recvfrom() failed with error code : %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}

			sensor->UpdateValue(sensor->buf);
		}

	}

	bool Framework::ProcessDataDirectory(const char* dataDirectory)
	{
		/* Flow variables */
		unsigned int i = 0;
		bool inputFound = false;

		/* look for network config files in directory */
		tinydir_dir dir;
		tinydir_open(&dir, dataDirectory);

		/* Process each file in turn */
		while (dir.has_next)
		{
			/* Get file information */
			tinydir_file dirFile;
			tinydir_readfile(&dir, &dirFile);

			/* Open the file as XML Document*/
			if (dirFile.is_dir == false)
			{
				
				tinyxml2::XMLDocument doc;
				doc.LoadFile(dirFile.path);

				tinyxml2::XMLElement* configElement = doc.FirstChildElement("config");

				/* Check to make sure the root tag is network */
				if (configElement != NULL)
				{
					/* Get Input Element */
					tinyxml2::XMLElement* inputElement = configElement->FirstChildElement("input");

					/* Loop throught each input to get configuration information */
					for (tinyxml2::XMLElement* e = inputElement->FirstChildElement("sensor"); e != NULL; e = e->NextSiblingElement("sensor"))
					{
						/* Sensor config found */
						SensorInput* newSensorInput = new SensorInput();

						/* name */
						strcpy_s(newSensorInput->name, e->Attribute("name"));
						/* ip port */
						newSensorInput->sourcePort = e->IntAttribute("port");

						/* Add new sensor to framework */
						_inputs.push_back(newSensorInput);
					}

					/* Get Output Element */
					tinyxml2::XMLElement* outputElement = configElement->FirstChildElement("output");

					/* Loop throught each output to get configuration information */
					for (tinyxml2::XMLElement* e = outputElement->FirstChildElement("network"); e != NULL; e = e->NextSiblingElement("network"))
					{
						/* Network Output config found */
						NetworkOutput* newNetworkOutput = new NetworkOutput();

						/* name */
						strcpy_s(newNetworkOutput->name, e->Attribute("name"));
						/* ip address */
						strcpy_s(newNetworkOutput->destinationIPAddress, e->Attribute("ipaddress"));
						/* ip port */
						newNetworkOutput->destinationPort = e->IntAttribute("port");

						/* Get Structure Element */
						tinyxml2::XMLElement* structureElement = e->FirstChildElement("structure");

						/* Build structure with block size */
						newNetworkOutput->outputStructure = NetworkOutputStructure(structureElement->IntAttribute("count"));

						/* Reset incrementor */
						i = 0;

						/* Loop throught each sensor to get configuration information */
						for (tinyxml2::XMLElement* s = structureElement->FirstChildElement("sensor"); s != NULL; s = s->NextSiblingElement("sensor"))
						{
							/* name */
							strcpy_s(newNetworkOutput->outputStructure.blocks[i].name, s->Attribute("name"));
							/* size */
							newNetworkOutput->outputStructure.blocks[i].size = s->IntAttribute("size");
							/* offset */
							newNetworkOutput->outputStructure.blocks[i].offset = s->IntAttribute("offset");

							/* Accumlate the total output block size */
							newNetworkOutput->outputStructure.totalSize += newNetworkOutput->outputStructure.blocks[i].size;

							/* Check to ensure that there is an input available to match otherwise we can't proceed */
							for (std::vector<SensorInput*>::iterator inputIt = _inputs.begin(); inputIt != _inputs.end(); ++inputIt)
							{
								SensorInput* sensorInput = (SensorInput*)*inputIt;

								if (strcmp(newNetworkOutput->outputStructure.blocks[i].name, sensorInput->name) == 0)
								{
									inputFound = true;
								}
							}

							if (inputFound == false)
							{
								printf("ERROR: Parsing Config file. Input Missing!\n");
								tinydir_close(&dir);
								return false;
							}
							else
							{
								/* Reset for next pass */
								inputFound = false;
							}


							/* increment for next block */
							i++;
						}

						/* Add new output to framework */
						_outputs.push_back(newNetworkOutput);
					}
				}
			}
			tinydir_next(&dir);
		}

		tinydir_close(&dir);

		return true;
	}
}


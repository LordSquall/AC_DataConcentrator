#include "framework.h"

namespace AC_SensorModels
{
	Framework::Framework()
	{

	}

	bool Framework::Initialise()
	{
		/* Local variables */
		bool result = false;

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

		/* Initialise each sensor */
		for (std::vector<SensorInput*>::iterator it = _inputs.begin(); it != _inputs.end(); ++it)
		{
			SensorInput* inputSensor = (SensorInput*)*it;

			/* Print out sensor information */
			printf("Sensor: %s on port %d\n", inputSensor->name, inputSensor->sourcePort);
		
			/* Open Sockets for each sensor */
			//sensor->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			//if (sensor->socket == SOCKET_ERROR)
			//{
			//	printf("ERROR: Unable to open Socket\n");
			//	return false;
			//}

			///* Setup address structure */
			//memset((char *)&sensor->socketAddr, 0, sizeof(sensor->socketAddr));
			//sensor->socketAddr.sin_family = AF_INET;
			//sensor->socketAddr.sin_port = htons(sensor->destinationPort);
			//sensor->socketAddr.sin_addr.S_un.S_addr = inet_addr(sensor->destinationIPAddress);
			//			
			///* Create Threads for each sensor */
			//std::thread* thread = new std::thread(&Framework::SensorFunction, this, sensor);
		}

		return result;
	}

	bool Framework::Frame()
	{
		return true;
	}

	bool Framework::ProcessDataDirectory(const char* dataDirectory)
	{
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

				tinyxml2::XMLElement* networkElement = doc.FirstChildElement("network");

				/* Check to make sure the root tag is network */
				if (networkElement != NULL)
				{
					/* Get Input Element */
					tinyxml2::XMLElement* inputElement = networkElement->FirstChildElement("input");

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
				}
			}
			tinydir_next(&dir);
		}

		tinydir_close(&dir);

		return true;
	}
}


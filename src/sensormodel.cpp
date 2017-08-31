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
		/*result = ProcessDataDirectory("../data/sensors");
		if (result == false)
		{
			return result;
		}*/

		/* Initialise networking */
		printf("Initialising Winsock\n");
		if (WSAStartup(MAKEWORD(2, 2), &_wsa) != 0)
		{
			return false;
		}

		///* Initialise each sensor */
		//for (std::vector<Sensor*>::iterator it = _sensors.begin(); it != _sensors.end(); ++it)
		//{
		//	Sensor* sensor = (Sensor*)*it;

		//	/* Print out sensor information */
		//	printf("Sensor: %s IPAddr: %s:%d\n", sensor->name, sensor->destinationIPAddress, sensor->destinationPort);
		//
		//	/* Open Sockets for each sensor */
		//	sensor->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		//	if (sensor->socket == SOCKET_ERROR)
		//	{
		//		printf("ERROR: Unable to open Socket\n");
		//		return false;
		//	}

		//	/* Setup address structure */
		//	memset((char *)&sensor->socketAddr, 0, sizeof(sensor->socketAddr));
		//	sensor->socketAddr.sin_family = AF_INET;
		//	sensor->socketAddr.sin_port = htons(sensor->destinationPort);
		//	sensor->socketAddr.sin_addr.S_un.S_addr = inet_addr(sensor->destinationIPAddress);
		//				
		//	/* Create Threads for each sensor */
		//	std::thread* thread = new std::thread(&Framework::SensorFunction, this, sensor);
		//}

		return result;
	}

	bool Framework::Frame()
	{
		return true;
	}

	bool Framework::ProcessDataDirectory(const char* dataDirectory)
	{
		/* look for sensor config files in directory */
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

				/* Check to make sure the root tag is sensor */
				if (doc.FirstChildElement("sensor") != NULL)
				{
					///* Sensor config found */
					//Sensor* newSensor = new Sensor();
					///* name */
					//strcpy_s(newSensor->name, doc.FirstChildElement("sensor")->FirstChildElement("name")->GetText());
					///* ip address */
					//strcpy_s(newSensor->destinationIPAddress, doc.FirstChildElement("sensor")->FirstChildElement("network")->Attribute("ipaddress"));
					///* ip port */
					//newSensor->destinationPort = atoi(doc.FirstChildElement("sensor")->FirstChildElement("network")->Attribute("port"));
				
					///* Add new sensor to framework */
					//_sensors.push_back(newSensor);
				}
			}
			tinydir_next(&dir);
		}

		tinydir_close(&dir);

		return true;
	}
}


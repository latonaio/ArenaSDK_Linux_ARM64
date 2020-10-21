/***************************************************************************************
 ***                                                                                 ***
 ***  Copyright (c) 2018, Lucid Vision Labs, Inc.                                    ***
 ***                                                                                 ***
 ***  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     ***
 ***  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       ***
 ***  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    ***
 ***  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         ***
 ***  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ***
 ***  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  ***
 ***  SOFTWARE.                                                                      ***
 ***                                                                                 ***
 ***************************************************************************************/
 // IpConfigUtility.cpp : Defines the entry point for the console application.
 //

#include "stdafx.h"

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
//#include <iphlpapi.h>
#include <arpa/inet.h>
#endif

#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>

#include "ArenaApi.h"

#if defined(_WIN32) || defined(_WIN64)
#include "GenICam.h"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "GenICam.h"
#pragma diagnostic pop
#endif


//command line input parser
class CliParser
{

public:

	CliParser(int& argc, char** argv)
	{
		for (int i = 1; i < argc; i++)
		{
			m_argTokens.push_back(std::string(argv[i]));
		}
	}

	~CliParser()
	{
		m_argTokens.clear();
	}

	//checks how many arguments were passed in
	size_t NumberOfArguments() const
	{
		return m_argTokens.size();
	}

	//checks if an argument exists
	bool ArgumentExists(const std::string& arg) const
	{
		auto result = std::find(m_argTokens.begin(), m_argTokens.end(), arg);
		return result != m_argTokens.end();
	}

	//returns the argument value associated with the arg flag
	//throws logic_error if argument not found
	const std::string& GetArgument(const std::string& arg) const
	{

		auto it = std::find(m_argTokens.begin(), m_argTokens.end(), arg);

		if (it != m_argTokens.end() && // if we found a flag token
			++it != m_argTokens.end()) // if there is a arg value token
		{
			return *it;
		}
		else
		{
			throw std::logic_error("Argument " + arg + " not found");
		}
	}

private:

	std::vector<std::string> m_argTokens;
};

int32_t SetPersistentIP(Arena::IDevice* pDevice, uint32_t ip, uint32_t subnet, uint32_t gateway)
{
	auto pNodeMap = pDevice->GetNodeMap();

	try
	{
		Arena::SetNodeValue<int64_t>(pNodeMap, "GevPersistentIPAddress", ip);
	}
	catch (GenICam::GenericException& e)
	{
		std::cout << "GevPersistentIPAddress is not writable, error = " << e.what() << std::endl;
		return -1;
	}

	try
	{
		Arena::SetNodeValue<int64_t>(pNodeMap, "GevPersistentSubnetMask", subnet);
	}
	catch (GenICam::GenericException& e)
	{
		std::cout << "GevPersistentSubnetMask is not writable, error = " << e.what() << std::endl;
		return -1;
	}

	try
	{
		Arena::SetNodeValue<int64_t>(pNodeMap, "GevPersistentDefaultGateway", gateway);
	}
	catch (GenICam::GenericException& e)
	{
		std::cout << "GevPersistentDefaultGateway is not writable, error = " << e.what() << std::endl;
		return -1;
	}

	return 0;
}

bool ToBool(std::string boolStr)
{
	bool bOut = false;
	std::istringstream(boolStr) >> std::boolalpha >> bOut;
	return bOut;
}

//Function to help print the list of cameras on the system
int PrintCameraList(Arena::ISystem* pSystem)
{
	auto devs = pSystem->GetDevices();
	static const size_t fillCount = 16;
	std::cout << "[index]\t";
	std::cout << std::left << std::setfill(' ') << std::setw(fillCount) << "MAC";
	std::cout << std::left << std::setfill(' ') << std::setw(fillCount) << "IP";
	std::cout << std::left << std::setfill(' ') << std::setw(fillCount) << "SUBNET";
	std::cout << std::left << std::setfill(' ') << std::setw(fillCount) << "GATEWAY";
	std::cout << "\tIP CONFIG" << std::endl;

	for (auto it = devs.begin(); it != devs.end(); it++)
	{
		auto dev = *it;
		std::cout << "[" << it - devs.begin() << "]\t";
		std::cout << std::hex << std::uppercase << std::left << std::setfill(' ') << std::setw(fillCount) << dev.MacAddress();
		std::cout << std::left << std::setfill(' ') << std::setw(fillCount) << dev.IpAddressStr();
		std::cout << std::left << std::setfill(' ') << std::setw(fillCount) << dev.SubnetMaskStr();
		std::cout << std::left << std::setfill(' ') << std::setw(fillCount) << dev.DefaultGatewayStr();
		std::cout << "\tDHCP= " << dev.IsDHCPConfigurationEnabled()
			<< " Persistent Ip= " << dev.IsPersistentIpConfigurationEnabled()
			<< " LLA = " << dev.IsLLAConfigurationEnabled() << std::endl;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	return 0;
}

// Find the index of the selected device, otherwise return -1
int FindSelectedDevice(Arena::ISystem* pSystem, CliParser& parser)
{
	size_t index = 0;
	uint64_t macAddress = 0;
	auto devs = pSystem->GetDevices();

	if (parser.ArgumentExists("-m"))
	{

		try
		{
			macAddress = std::stoull(parser.GetArgument("-m"), nullptr, 16);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	else
	{
		try
		{
			index = std::stol(parser.GetArgument("-i"), nullptr, 16);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			return -1;
		}

		if (index >= pSystem->GetDevices().size() || index < 0)
		{
			std::cout << "Device index is out of range!" << std::endl;
			return -1;
		}

		//in this case user just gave us the index directly.
		return static_cast<int>(index);
	}

	auto it = std::find_if(begin(devs), end(devs), [&macAddress](Arena::DeviceInfo devInfo) {
		return devInfo.MacAddress() == macAddress;
	});

	if (it == std::end(devs))
	{
		std::cout << "Couldn't find device!" << std::endl;
		return -1;
	}

	//seems we found the device, calculate the index
	return static_cast<int>(it - devs.begin());
}

uint32_t AddressStrToUint32(std::string address)
{
	uint32_t uAddress = 0;
	if (inet_pton(AF_INET, address.c_str(), &uAddress) > 0)
	{
		return ntohl(uAddress);
	}
	else
	{
		//in this case its not a valid address
		throw std::logic_error(address + " is not a valid address!");
	}
}

//temporarily force an IP address to the camera.
int HandleForceIP(Arena::ISystem* pSystem, CliParser& parser)
{
	int ret = 0;
	if ((ret = FindSelectedDevice(pSystem, parser)) < 0)
	{
		return ret;
	}

	//above function returns the index of the found device
	size_t index = ret;

	std::string ipStr, subnetStr, gatewayStr;

	try
	{
		//these must be set
		ipStr = parser.GetArgument("-a");

		subnetStr = parser.GetArgument("-s");
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}

	try
	{
		gatewayStr = parser.GetArgument("-g");
	}
	catch (std::exception& /*e*/)
	{
		std::cout << "No GatewaySelected, defaulting to 0.0.0.0" << std::endl;
		gatewayStr = "0.0.0.0";
	}

	try
	{
		pSystem->ForceIp(
			pSystem->GetDevices()[index].MacAddress(),
			AddressStrToUint32(ipStr),
			AddressStrToUint32(subnetStr),
			AddressStrToUint32(gatewayStr));
		std::cout << "Successfully Forced IP to: (" << ipStr << ", " << subnetStr << ", " << gatewayStr << ")" << std::endl;
	}
	catch (GenICam::GenericException& e)
	{
		std::cout << "Failed to force Ip to device with error:" << e.what() << std::endl;
		return -1;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
}

//set the persisten ip address to the camera
int HandlePersistentIP(Arena::ISystem* pSystem, CliParser& parser)
{
	int ret = 0;
	if ((ret = FindSelectedDevice(pSystem, parser)) < 0)
	{
		return ret;
	}

	//above function returns the index of the found device
	size_t index = ret;

	std::string ipStr, subnetStr, gatewayStr;

	try
	{
		//these must be set
		ipStr = parser.GetArgument("-a");

		subnetStr = parser.GetArgument("-s");
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}

	try
	{
		gatewayStr = parser.GetArgument("-g");
	}
	catch (std::exception& /*e*/)
	{
		std::cout << "No GatewaySelected, defaulting to 0.0.0.0" << std::endl;
		gatewayStr = "0.0.0.0";
	}

	Arena::IDevice* pDevice = nullptr;
	try
	{
		try
		{
			pDevice = pSystem->CreateDevice(pSystem->GetDevices()[index]);
		}
		catch (GenICam::GenericException& e)
		{
			std::cout << "Failed to get a device with error: " << e.what() << std::endl;
			// if there is no device, there is nothing that we can do further, so return immediately 
			return -1;
		}

		try
		{
			ret = SetPersistentIP(pDevice, AddressStrToUint32(ipStr), AddressStrToUint32(subnetStr), AddressStrToUint32(gatewayStr));
		}
		catch (GenICam::GenericException& e)
		{
			std::cout << "Failed to set a persistent IP with error: " << e.what() << std::endl;
			ret = -1;
		}

		try
		{
			Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "GevCurrentIPConfigurationPersistentIP", true);
			// std::cout << "Successfully set Persistent IP configuration to = " << std::boolalpha << true << std::endl;
		}
		catch (GenICam::GenericException& e)
		{
			std::cout << "GevCurrentIPConfigurationPersistentIP is not writable, error = " << e.what() << std::endl;
			ret = -1;
		}

		try
		{
			Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "GevCurrentIPConfigurationDHCP", false);
			// std::cout << "Successfully set DHCP configuration to = " << std::boolalpha << false << std::endl;
		}
		catch (GenICam::GenericException& e)
		{
			std::cout << "GevCurrentIPConfigurationDHCP is not writable, error = " << e.what() << std::endl;
			ret = -1;
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		ret = -1;
	}

	if (ret == 0)
	{
		std::cout << "Successfully set Persistent Ip to: (" << ipStr << ", " << subnetStr << ", " << gatewayStr << ")" << std::endl;
	}

	if (pDevice != nullptr)
	{
		try
		{
			pSystem->DestroyDevice(pDevice);
			pDevice = nullptr;
		}
		catch (GenICam::GenericException& e)
		{
			std::cout << "Failed to destroy device with error: " << e.what() << std::endl;
			ret = -1;
		}
	}

	return ret;
}

int HandleDHCP(Arena::ISystem* pSystem, CliParser& parser)
{
	int ret = 0;
	if ((ret = FindSelectedDevice(pSystem, parser)) < 0)
	{
		return ret;
	}

	//above function returns the index of the found device
	size_t index = ret;

	Arena::IDevice* pDevice = nullptr;

	try
	{
		pDevice = pSystem->CreateDevice(pSystem->GetDevices()[index]);
	}
	catch (GenICam::GenericException& e)
	{
		std::cout << "Failed to get a device with error: " << e.what() << std::endl;
		return -1;
	}

	//reset return count to success
	ret = 0;

	try
	{
		auto enablePersistentIP = false;
		try
		{
			Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "GevCurrentIPConfigurationPersistentIP", enablePersistentIP);
		}
		catch (GenICam::GenericException& e)
		{
			std::cout << "GevCurrentIPConfigurationPersistentIP is not writable, error = " << e.what() << std::endl;
			ret = -1;
		}
	}
	catch (std::exception& /*e*/)
	{
		std::cout << "Leaving Persistent IP configuration unchanged" << std::endl;
	}

	try
	{
		auto enableDHCP = true;
		try
		{
			Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "GevCurrentIPConfigurationDHCP", enableDHCP);
			std::cout << "Successfully set DHCP configuration to = " << std::boolalpha << enableDHCP << std::endl;
		}
		catch (GenICam::GenericException& e)
		{
			std::cout << "GevCurrentIPConfigurationDHCP is not writable, error = " << e.what() << std::endl;
			ret = -1;
		}
	}
	catch (std::exception& /*e*/)
	{
		std::cout << "Leaving DHCP configuration unchanged" << std::endl;
	}

	try
	{
		pSystem->DestroyDevice(pDevice);
	}
	catch (GenICam::GenericException& e)
	{
		std::cout << "Failed to destroy device with error: " << e.what() << std::endl;
		ret = -1;
	}

	return ret;
}

int HandleIPConfigurationOptions(Arena::ISystem* pSystem, CliParser& parser)
{
	int ret = 0;
	if ((ret = FindSelectedDevice(pSystem, parser)) < 0)
	{
		return ret;
	}

	//above function returns the index of the found device
	size_t index = ret;

	Arena::IDevice* pDevice = nullptr;

	try
	{
		pDevice = pSystem->CreateDevice(pSystem->GetDevices()[index]);
	}
	catch (GenICam::GenericException& e)
	{
		std::cout << "Failed to get a device with error: " << e.what() << std::endl;
		return -1;
	}

	//reset return count to success
	ret = 0;

	try
	{
		auto enablePersistentIP = ToBool(parser.GetArgument("-p"));
		try
		{
			Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "GevCurrentIPConfigurationPersistentIP", enablePersistentIP);
			std::cout << "Successfully set Persistent IP configuration to = " << std::boolalpha << enablePersistentIP << std::endl;
		}
		catch (GenICam::GenericException& e)
		{
			std::cout << "GevCurrentIPConfigurationPersistentIP is not writable, error = " << e.what() << std::endl;
			ret = -1;
		}
	}
	catch (std::exception& /*e*/)
	{
		std::cout << "Leaving Persistent IP configuration unchanged" << std::endl;
	}

	try
	{
		auto enableDHCP = ToBool(parser.GetArgument("-d"));
		try
		{
			Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "GevCurrentIPConfigurationDHCP", enableDHCP);
			std::cout << "Successfully set DHCP configuration to = " << std::boolalpha << enableDHCP << std::endl;
		}
		catch (GenICam::GenericException& e)
		{
			std::cout << "GevCurrentIPConfigurationDHCP is not writable, error = " << e.what() << std::endl;
			ret = -1;
		}
	}
	catch (std::exception& /*e*/)
	{
		std::cout << "Leaving DHCP configuration unchanged" << std::endl;
	}

	pSystem->DestroyDevice(pDevice);

	return ret;
}

void PrintUsage()
{
	std::cout << std::endl;
	std::cout << "Usage: IpConfigUtility.exe /<command> -<arg> <arg_value>" << std::endl
		<< std::endl;

	std::cout << "/list: \t Lists all the devices connected to this machine" << std::endl;
	std::cout << std::endl;
	std::cout << "/config: \t This toggles the current IP configuration scheme for a device." << std::endl;
	std::cout << " \t The arguments can be: (mandatory args: [-m or -i])" << std::endl;
	std::cout << " \t -m: The mac address of the device to change the configuration on (eg. \"0x1C0FAF000001\")" << std::endl;
	std::cout << " \t -i: The index of the device to change the configuration on (eg. 0 or 1 or 3 etc.)" << std::endl;
	std::cout << " \t -p: Enable/Disable persistent IP (eg. \"true\")" << std::endl;
	std::cout << " \t -d: Enable/Disable DHCP (eg. \"true\")" << std::endl;
	std::cout << " \t For example $> IpConfigUtility.exe /config -i 0 -p true -d false" << std::endl;
	std::cout << std::endl;
	std::cout << "/force: \t This command forces an ip to the camera. It takes up to 4 arguments" << std::endl;
	std::cout << " \t The arguments can be: (mandatory args: [-m or -i],-a,-s)" << std::endl;
	std::cout << " \t -m: The mac address of the device to force (eg. \"0x1C0FAF000001\")" << std::endl;
	std::cout << " \t -i: The index of the device to force ip to (eg. 0 or 1 or 3 etc.)" << std::endl;
	std::cout << " \t -a: The ip address to force (eg. \"169.254.1.1\")" << std::endl;
	std::cout << " \t -s: The subnet to force (eg. \"255.255.0.0\")" << std::endl;
	std::cout << " \t -g: The gateway to force (eg. \"169.254.1.1\")" << std::endl;
	std::cout << " \t For example $> IpConfigUtility.exe /force -i 0 -a \"169.254.1.1\" -s \"255.255.0.0\"" << std::endl;
	std::cout << std::endl;
	std::cout << "/persist:\t This command sets a peristent ip to the camera. It takes up to 4 arguments" << std::endl;
	std::cout << " \t The arguments can be: (mandatory args: [-m or -i],-a,-s)" << std::endl;
	std::cout << " \t -m: The mac address of the device to change the persistent ip on (eg. \"0x1C0FAF000001\")" << std::endl;
	std::cout << " \t -i: The index of the device to change the persistent ip on (eg. 0 or 1 or 3 etc.)" << std::endl;
	std::cout << " \t -a: The new persistent ip address (eg. \"169.254.1.1\")" << std::endl;
	std::cout << " \t -s: The new perisitent subnet (eg. \"255.255.0.0\")" << std::endl;
	std::cout << " \t -g: The new persistent gateway (eg. \"169.254.1.1\")" << std::endl;
	std::cout << " \t For example $> IpConfigUtility.exe /persist -i 0 -a \"169.254.1.1\" -s \"255.255.0.0\"" << std::endl;
	std::cout << std::endl;
	std::cout << "/dhcp: \t This command sets the camera to dhcp mode." << std::endl;
	std::cout << " \t The arguments can be: (mandatory args: [-m or -i],-a,-s)" << std::endl;
	std::cout << " \t -m: The mac address of the device to change the persistent ip on (eg. \"0x1C0FAF000001\")" << std::endl;
	std::cout << " \t -i: The index of the device to change the persistent ip on (eg. 0 or 1 or 3 etc.)" << std::endl;
	std::cout << " \t For example $> IpConfigUtility.exe /dhcp -i 0" << std::endl;
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	int ret = -1;
	
	try
	{
		CliParser parser(argc, argv);

		auto pSystem = Arena::OpenSystem();

		pSystem->UpdateDevices(1000);

		if (parser.ArgumentExists("/list"))
		{
			ret = PrintCameraList(pSystem);
		}
		else if (parser.ArgumentExists("/force"))
		{
			ret = HandleForceIP(pSystem, parser);
		}
		else if (parser.ArgumentExists("/persist"))
		{
			ret = HandlePersistentIP(pSystem, parser);
		}
		else if (parser.ArgumentExists("/config"))
		{
			ret = HandleIPConfigurationOptions(pSystem, parser);
		}
		else if (parser.ArgumentExists("/dhcp"))
		{
			ret = HandleDHCP(pSystem, parser);
		}
		else
		{
			std::cout << "Unknown command!" << std::endl;
			PrintUsage();
			ret = 0;
		}

		if (ret != 0)
		{
			PrintUsage();
		}

		//cleanup the Arena System
		Arena::CloseSystem(pSystem);
	}
	catch (GenICam::GenericException& e)
	{
		std::cout << "Arena Error: " << e.what() << std::endl;
		ret = -1;
	}
	catch (std::exception& e)
	{
		std::cout << "Std Error: " << e.what() << std::endl;
		ret = -1;
	}

	std::cout << "Press enter to complete\n";
	std::getchar();

	return ret;
}

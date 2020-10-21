/***************************************************************************************
 ***                                                                                 ***
 ***  Copyright (c) 2019, Lucid Vision Labs, Inc.                                    ***
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
#pragma once
#include <string>

namespace Arena
{
	/**
	 * @class DeviceInfo
	 *
	 * Device information objects contain device discovery information used to
	 * find, filter, and create devices.
	 *
	 * A list of device information objects is retrieved and maintained in the
	 * system (Arena::ISystem). A device information object provides a device's
	 * discovery information:
	 *  - model and vendor names
	 *  - serial number
	 *  - IP and MAC addresses
	 *  - subnet mask
	 *
	 * Discovery information is acquired prior to device creation and used
	 * primarily to filter and find specific devices. A std::vector of
	 * Arena::DeviceInfo objects is retrieved from the system (via
	 * Arena::ISystem::GetDevices). They are then individually passed back to the
	 * system to create devices (via Arena::ISystem::CreateDevice).
	 *
	 * The list of DeviceInfos is returned as a std::vector in order to make use
	 * of STL filter and search tools. The following code snippet demonstrates
	 * searching for a device with a specific serial number:
	 *
	 * \code{.cpp}
	 * 	// searching for a specific device
	 * 	{
	 * 		GenICam::gcstring serialToFind = "12345";
	 * 		std::vector<Arena::DeviceInfo> devs = pSystem->GetDevices();
	 * 		
	 * 		auto it = std::find_if(begin(devs), end(devs), [&serialToFind](Arena::DeviceInfo deviceInfo)
	 * 		{
	 * 			return deviceInfo.SerialNumber() == serialToFind;
	 * 		});
	 * 	}
	 * \endcode
	 *
	 * @see 
	 *  - Arena::ISystem
	 *  - Arena::DeviceInfo
	 */
	class ARENA_API DeviceInfo
	{
	public:

		/**
		 * @fn virtual GenICam::gcstring ModelName()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - Model name of the device
		 *
		 * <B> ModelName </B> gets the model name of a device.
		 *
		 * Model names are used to differentiate between products. Below are a
		 * few examples of Lucid Vision model names:
		 *  - PHX050S-MC
		 *  - PHX032S-CC
		 *  - TRI032S-MC
		 *
		 * The model name returned by this getter is the same as the one received
		 * in the GigE Vision discovery acknowledgement.
		 */
		virtual GenICam::gcstring ModelName();

		/**
		 * @fn virtual GenICam::gcstring VendorName()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - Vendor name of the device
		 *
		 * <B> VendorName </B> gets the vendor/manufacturer name of a device.
		 * Vendor names differentiate between device vendors/manufacturers. Lucid
		 * devices return 'Lucid Vision Labs'.
		 *
		 * The vendor name returned by this getter is the same as the one
		 * received in the GigE Vision discovery acknowledgement.
		 */
		virtual GenICam::gcstring VendorName();

		/**
		 * @fn virtual GenICam::gcstring SerialNumber()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - Serial number of the device
		 *
		 * <B> SerialNumber </B> gets the serial number of a device. A serial
		 * number differentiates between devices. Each Lucid device has a unique
		 * serial number. Lucid serial numbers are numeric, but the serial
		 * numbers of other vendors may be alphanumeric.
		 *
		 * The serial number returned by this getter is the same as the one
		 * received in the GigE Vision discovery acknowledgement.
		 *
		 * @warning 
		 *  - Serial numbers from different manufacturers may overlap
		 */
		virtual GenICam::gcstring SerialNumber();

		/**
		 * @fn virtual uint32_t IpAddress()
		 *
		 * @return 
		 *  - Type: uint32_t
		 *  - IP address of the device as an integer
		 *
		 * <B> IpAddress </B> gets the IP address of a device on the network,
		 * returning it as its raw integer value.
		 *
		 * The GigE Vision specification only allows for IPv4 IP addresses. The
		 * IP address is represented by the lower 32 bits of the integer
		 * returned. Both <B> IpAddress </B> and <B> IpAddressStr </B> represent
		 * the same IP address. Where <B> IpAddress </B> returns the value in its
		 * raw integer format, <B> IpAddressStr </B> returns it as a more
		 * human-readable string:
		 *  - <B> IpAddress </B>: 0xA9FE0101 
		 *  - <B> IpAddressStr </B>: 169.254.1.1
		 *
		 * A device may have its IP address, subnet mask, and default gateway
		 * assigned by LLA or DHCP, set as persistent, or temporarily forced.
		 * They can be checked through the main node map
		 * (Arena::IDevice::GetNodeMap, 'GevCurrentIPAddress',
		 * 'GevCurrentSubnetMask', 'GevCurrentDefaultGateway').
		 *
		 * DHCP (Arena::IDevice::GetNodeMap, 'GevCurrentIPConfigurationDHCP') and
		 * IP persistence (Arena::IDevice::GetNodeMap,
		 * 'GevCurrentIPConfigurationPersistentIP') can be enabled/disabled
		 * through the node map. If both are enabled, a device will default to
		 * its persistent IP settings. If neither, it will default to LLA
		 * (Arena::IDevice::GetNodeMap, 'GevCurrentIPConfigurationLLA'), which cannot
		 * be disabled.
		 *
		 * In order to configure a device to use a persistent IP configuration,
		 * not only must IP persistence be enabled (Arena::IDevice::GetNodeMap,
		 * 'GevCurrentIPConfigurationPersistentIP'), but the IP address
		 * (Arena::IDevice::GetNodeMap, 'GevPersistentIPAddress'), subnet mask
		 * (Arena::IDevice::GetNodeMap, 'GevPersistentSubnetMask'), and default
		 * gateway (Arena::IDevice::GetNodeMap, 'GevPersistentDefaultGateway')
		 * must be set.
		 *
		 * Forcing an IP (Arena::IDevice::ForceIP) temporarily changes an IP
		 * address, subnet mask, and default gateway of a device. A forced IP
		 * configuration will reset on a device reboot
		 * (Arena::IDevice::GetNodeMap, 'DeviceReset').
		 *
		 * The IP address returned by this getter is the same as the one received
		 * in the GigE Vision discovery acknowledgement.
		 *
		 * @warning 
		 *  - Represents same information as Arena::DeviceInfo::IpAddressStr
		 *  - A persistent IP may be quicker to enumerate than DHCP, which
		 *    should be faster than LLA
		 *
		 * @see 
		 *  - Arena::IDevice::GetNodeMap
		 *  - Arena::IDevice::ForceIP
		 *  - Arena::DeviceInfo::IpAddressStr
		 */
		virtual uint32_t IpAddress();

		/**
		 * @fn virtual GenICam::gcstring IpAddressStr()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - IP address of the device as a string
		 *
		 * <B> IpAddressStr </B> gets the IP address of a device on the network,
		 * returning it as a string.
		 *
		 * The GigE Vision specification only allows for IPv4 IP addresses. Both
		 * <B> IpAddress </B> and <B> IpAddressStr </B> represent the same IP
		 * address. Where <B> IpAddress </B> returns the value in its raw integer
		 * format, <B> IpAddressStr </B> returns it as a more human-readable
		 * string:
		 *  - <B> IpAddress </B>: 0xA9FE0101 
		 *  - <B> IpAddressStr </B>: 169.254.1.1
		 *
		 * A device may have its IP address, subnet mask, and default gateway
		 * assigned by LLA or DHCP, set as persistent, or temporarily forced.
		 * They can be checked through the main node map
		 * (Arena::IDevice::GetNodeMap, 'GevCurrentIPAddress',
		 * 'GevCurrentSubnetMask', 'GevCurrentDefaultGateway').
		 *
		 * DHCP (Arena::IDevice::GetNodeMap, 'GevCurrentIPConfigurationDHCP') and
		 * IP persistence (Arena::IDevice::GetNodeMap,
		 * 'GevCurrentIPConfigurationPersistentIP') can be enabled/disabled
		 * through the node map. If both are enabled, a device will default to
		 * its persistent IP settings. If neither, it will default to LLA
		 * (Arena::IDevice::GetNodeMap, 'GevCurrentIPConfigurationLLA'), which cannot
		 * be disabled.
		 *
		 * In order to configure a device to use a persistent IP configuration,
		 * not only must IP persistence be enabled (Arena::IDevice::GetNodeMap,
		 * 'GevCurrentIPConfigurationPersistentIP'), but the IP address
		 * (Arena::IDevice::GetNodeMap, 'GevPersistentIPAddress'), subnet mask
		 * (Arena::IDevice::GetNodeMap, 'GevPersistentSubnetMask'), and default
		 * gateway (Arena::IDevice::GetNodeMap, 'GevPersistentDefaultGateway')
		 * must be set.
		 *
		 * Forcing an IP (Arena::IDevice::ForceIP) temporarily changes an IP
		 * address, subnet mask, and default gateway of a device. A forced IP
		 * configuration will reset on a device reboot
		 * (Arena::IDevice::GetNodeMap, 'DeviceReset').
		 *
		 * The IP address returned by this getter is the same as the one received
		 * in the GigE Vision discovery acknowledgement, but as a dot-separated
		 * string.
		 *
		 * @warning 
		 *  - Represents same information as Arena::DeviceInfo::IpAddress
		 *  - A persistent IP may be quicker to enumerate than DHCP, which
		 *    should be faster than LLA
		 *
		 * @see 
		 *  - Arena::IDevice::GetNodeMap
		 *  - Arena::IDevice::ForceIP
		 *  - Arena::DeviceInfo::IpAddress
		 */
		virtual GenICam::gcstring IpAddressStr();

		/**
		 * @fn virtual uint32_t SubnetMask()
		 *
		 * @return 
		 *  - Type: uint32_t
		 *  - Subnet mask of the device as an integer
		 *
		 * <B> SubnetMask </B> gets the subnet mask of a device on the network,
		 * returning it as its raw integer value.
		 *
		 * The GigE Vision specification only allows for IPv4 subnet masks. The
		 * subnet mask is represented by the lower 32 bits of the integer
		 * returned. Both <B> SubnetMask </B> and <B> SubnetMaskStr </B>
		 * represent the same subnet mask. Where <B> SubnetMask </B> returns the
		 * value in its raw integer format, <B> SubnetMaskStr </B> returns it as
		 * a more human-readable string:
		 *  - <B> SubnetMask </B>: 0xFFFF0000 
		 *  - <B> SubnetMaskStr </B>: 255.255.0.0
		 *
		 * A device may have its IP address, subnet mask, and default gateway
		 * assigned by LLA or DHCP, set as persistent, or temporarily forced.
		 * They can be checked through the main node map
		 * (Arena::IDevice::GetNodeMap, 'GevCurrentIPAddress',
		 * 'GevCurrentSubnetMask', 'GevCurrentDefaultGateway').
		 *
		 * DHCP (Arena::IDevice::GetNodeMap, 'GevCurrentIPConfigurationDHCP') and
		 * IP persistence (Arena::IDevice::GetNodeMap,
		 * 'GevCurrentIPConfigurationPersistentIP') can be enabled/disabled
		 * through the node map. If both are enabled, a device will default to
		 * its persistent IP settings. If neither, it will default to LLA
		 * (Arena::IDevice::GetNodeMap, 'GevCurrentIPConfigurationLLA'), which cannot
		 * be disabled.
		 *
		 * In order to configure a device to use a persistent IP configuration,
		 * not only must IP persistence be enabled (Arena::IDevice::GetNodeMap,
		 * 'GevCurrentIPConfigurationPersistentIP'), but the IP address
		 * (Arena::IDevice::GetNodeMap, 'GevPersistentIPAddress'), subnet mask
		 * (Arena::IDevice::GetNodeMap, 'GevPersistentSubnetMask'), and default
		 * gateway (Arena::IDevice::GetNodeMap, 'GevPersistentDefaultGateway')
		 * must be set.
		 *
		 * Forcing an IP (Arena::IDevice::ForceIP) temporarily changes an IP
		 * address, subnet mask, and default gateway of a device. A forced IP
		 * configuration will reset on a device reboot
		 * (Arena::IDevice::GetNodeMap, 'DeviceReset').
		 *
		 * The subnet mask returned by this getter is the same as the one
		 * received in the GigE Vision discovery acknowledgement.
		 *
		 * @warning 
		 *  - Represents same information as Arena::DeviceInfo::SubnetMaskStr
		 *  - A persistent IP may be quicker to enumerate than DHCP, which
		 *    should be faster than LLA
		 *
		 * @see 
		 *  - Arena::IDevice::GetNodeMap
		 *  - Arena::IDevice::ForceIP
		 *  - Arena::DeviceInfo::SubnetMaskStr
		 */
		virtual uint32_t SubnetMask();

		/**
		 * @fn virtual GenICam::gcstring SubnetMaskStr()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - Subnet mask of the device as a string
		 *
		 * <B> SubnetMaskStr </B> gets the subnet mask of a device on the
		 * network, returning it as a string.
		 *
		 * The GigE Vision specification only allows for IPv4 subnet masks. Both
		 * <B> SubnetMask </B> and <B> SubnetMaskStr </B> represent the same
		 * subnet mask. Where <B> SubnetMask </B> returns the value in its raw
		 * integer format, <B> SubnetMaskStr </B> returns it as a more
		 * human-readable string:
		 *  - <B> SubnetMask </B>: 0xFFFF0000 
		 *  - <B> SUbnetMaskStr </B>: 255.255.0.0
		 *
		 * A device may have its IP address, subnet mask, and default gateway
		 * assigned by LLA or DHCP, set as persistent, or temporarily forced.
		 * They can be checked through the main node map
		 * (Arena::IDevice::GetNodeMap, 'GevCurrentIPAddress',
		 * 'GevCurrentSubnetMask', 'GevCurrentDefaultGateway').
		 *
		 * DHCP (Arena::IDevice::GetNodeMap, 'GevCurrentIPConfigurationDHCP') and
		 * IP persistence (Arena::IDevice::GetNodeMap,
		 * 'GevCurrentIPConfigurationPersistentIP') can be enabled/disabled
		 * through the node map. If both are enabled, a device will default to
		 * its persistent IP settings. If neither, it will default to LLA
		 * (Arena::IDevice::GetNodeMap, 'GevCurrentIPConfigurationLLA'), which cannot
		 * be disabled.
		 *
		 * In order to configure a device to use a persistent IP configuration,
		 * not only must IP persistence be enabled (Arena::IDevice::GetNodeMap,
		 * 'GevCurrentIPConfigurationPersistentIP'), but the IP address
		 * (Arena::IDevice::GetNodeMap, 'GevPersistentIPAddress'), subnet mask
		 * (Arena::IDevice::GetNodeMap, 'GevPersistentSubnetMask'), and default
		 * gateway (Arena::IDevice::GetNodeMap, 'GevPersistentDefaultGateway')
		 * must be set.
		 *
		 * Forcing an IP (Arena::IDevice::ForceIP) temporarily changes an IP
		 * address, subnet mask, and default gateway of a device. A forced IP
		 * configuration will reset on a device reboot
		 * (Arena::IDevice::GetNodeMap, 'DeviceReset').
		 *
		 * The subnet mask returned by this getter is the same as the one
		 * received in the GigE Vision discovery acknowledgement, but as a
		 * dot-separated string.
		 *
		 * @warning 
		 *  - Represents same information as Arena::DeviceInfo::SubnetMask
		 *  - A persistent IP may be quicker to enumerate than DHCP, which
		 *    should be faster than LLA
		 *
		 * @see 
		 *  - Arena::IDevice::GetNodeMap
		 *  - Arena::IDevice::ForceIP
		 *  - Arena::DeviceInfo::SubnetMask
		 */
		virtual GenICam::gcstring SubnetMaskStr();

		/**
		 * @fn virtual uint32_t DefaultGateway()
		 *
		 * @return 
		 *  - Type: uint32_t
		 *  - The default gateway
		 *
		 * <B> DefaultGateway </B> retrieves the default gateway of the device.
		 */
		virtual uint32_t DefaultGateway();

		/**
		 * @fn virtual GenICam::gcstring DefaultGatewayStr()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - The default gateway
		 *
		 * <B> DefaultGateway </B> retrieves the default gateway of the device as
		 * a string.
		 */
		virtual GenICam::gcstring DefaultGatewayStr();

		/**
		 * @fn virtual uint64_t MacAddress()
		 *
		 * @return 
		 *  - Type: uint64_t
		 *  - MAC address of the device as an integer
		 *
		 * <B> MacAddress </B> gets the MAC address of a device on the network,
		 * returning it as its raw integer value.
		 *
		 * Both <B> MacAddress </B> and <B> MacAddressStr </B> represent the same
		 * MAC address. Where <B> MacAddress </B> returns the value in its raw
		 * integer format, <B> MacAddressStr </B> returns it as a more
		 * human-readable string:
		 *  - <B> MacAddress </B>: 0x1C0FAF010101 
		 *  - <B> MacAddressStr </B>: 1C:0F:AF:01:01:01
		 *
		 * The MAC address returned by this getter is the same as the one
		 * received in the GigE Vision discovery acknowledgement.
		 *
		 * @warning 
		 *  - Represents same information as Arena::DeviceInfo::MacAddressStr
		 *
		 * @see 
		 *  - Arena::DeviceInfo::MacAddressStr
		 */
		virtual uint64_t MacAddress();

		/**
		 * @fn virtual GenICam::gcstring MacAddressStr()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - MAC address of the device as a string
		 *
		 * <B> MacAddressStr </B> gets the MAC address of a device on the
		 * network, returning it as a string.
		 *
		 * Both <B> MacAddress </B> and <B> MacAddressStr </B> represent the same
		 * MAC address. Where <B> MacAddress </B> returns the value in its raw
		 * integer format, <B> MacAddressStr </B> returns it as a more
		 * human-readable string:
		 *  - <B> MacAddress </B>: 0x1C0FAF010101 
		 *  - <B> MacAddressStr </B>: 1C:0F:AF:01:01:01
		 *
		 * The MAC address returned by this getter is the same as the one
		 * received in the GigE Vision discovery acknowledgement.
		 *
		 * @warning 
		 *  - Represents same information as Arena::DeviceInfo::MacAddress
		 *
		 * @see 
		 *  - Arena::DeviceInfo::MacAddress
		 */
		virtual GenICam::gcstring MacAddressStr();

		/**
		 * @fn virtual GenICam::gcstring UserDefinedName()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - User-defined name of a device
		 *
		 * <B> UserDefinedName </B> gets the user-defined name of a device. If
		 * supported, it is a customizable string with a maximum of 16 bytes that
		 * can be used to identify a device (Arena::IDevice::GetNodeMap,
		 * 'DeviceUserID').
		 *
		 * The user-defined name returned by this getter is the same as the one
		 * received in the GigE Vision discovery acknowledgement.
		 *
		 * @warning 
		 *  - Not necessarily supported
		 *
		 * @see 
		 *  - Arena::IDevice::GetNodeMap
		 */
		virtual GenICam::gcstring UserDefinedName();

		/**
		 * @fn virtual bool IsDHCPConfigurationEnabled()
		 *
		 * @return 
		 *  - Type: bool
		 *  - True if DHCP enabled
		 *  - Otherwise, false
		 *
		 * <B> IsDHCPConfigurationEnabled </B> retrieves whether DHCP is enabled
		 * on the device.
		 */
		virtual bool IsDHCPConfigurationEnabled();

		/**
		 * @fn virtual bool IsPersistentIpConfigurationEnabled()
		 *
		 * @return 
		 *  - Type: bool
		 *  - True if persistent IP enabled
		 *  - Otherwise, false
		 *
		 * <B> IsPersistentIpConfigurationEnabled </B> retrieves whether
		 * persistent IP is enabled on the device.
		 */
		virtual bool IsPersistentIpConfigurationEnabled();

		/**
		 * @fn virtual bool IsLLAConfigurationEnabled()
		 *
		 * @return 
		 *  - Type: bool
		 *  - True if LLA enabled
		 *  - Otherwise, false
		 *
		 * <B> IsLLAConfigurationEnabled </B> retrieves whether LLA is enabled on
		 * the device.
		 */
		virtual bool IsLLAConfigurationEnabled();

		/**
		 * @fn virtual GenICam::gcstring DeviceVersion()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - Firmware version of the device
		 *
		 * <B> DeviceVersion</B> retrieves the version of the device currently
		 * running on the device. For Lucid devices, this refers to firmware
		 * version.
		 */
		virtual GenICam::gcstring DeviceVersion();

		/**
		 * @fn DeviceInfo()
		 *
		 * An empty constructor
		 *
		 * @warning 
		 *  - Does not sufficiently initialize DeviceInfo
		 */
		DeviceInfo();

		/**
		 * @fn DeviceInfo(const DeviceInfo& deviceInfo)
		 *
		 * A copy constructor
		 *
		 * @param deviceInfo
		 *  - Type: const DeviceInfo&
		 *  - Device information object to copy
		 */
		DeviceInfo(const DeviceInfo& deviceInfo);

		/**
		 * @fn virtual ~DeviceInfo()
		 *
		 * A destructor
		 */
		virtual ~DeviceInfo();

		/**
		 * @fn virtual DeviceInfo& operator=(DeviceInfo deviceInfo)
		 *
		 * A copy assignment operator
		 *
		 * @param deviceInfo
		 *  - Type: DeviceInfo
		 *  - Device information object to copy
		 *
		 * @return 
		 *  - Type: DeviceInfo&
		 *  - Copied device information object
		 */
		virtual DeviceInfo& operator=(DeviceInfo deviceInfo);

	protected:

		friend class System;
		void* m_pData;
	};
}
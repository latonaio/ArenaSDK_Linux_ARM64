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
	 * @class InterfaceInfo
	 *
	 * Interface information objects contain interface information used to find,
	 * filter, and create devices on a specific interface on the host.
	 *
	 * A list of interface information objects is retrieved and maintained in the
	 * system (Arena::ISystem). An interface information object provides a:
	 *  - IP and MAC addresses
	 *  - subnet mask
	 *
	 * Interface information is acquired prior to device creation and used
	 * primarily to filter and find specific interface. A std::vector of
	 * Arena::InterfaceInfo objects is retrieved from the system (via
	 * Arena::ISystem::GetInterfaces).
	 *
	 * The list of InterfaceInfos is returned as a std::vector in order to make
	 * use of STL filter and search tools. The following code snippet
	 * demonstrates searching for an interface with a specific Mac address:
	 *
	 * \code{.cpp}
	 * 	// searching for a specific interface
	 * 	{
	 * 		GenICam::gcstring macToFind = "ab:cd:ef:aa:aa:aa";
	 * 		std::vector<Arena::InterfaceInfo> intf = pSystem->GetInterfaces();
	 * 		
	 * 		auto it = std::find_if(begin(intf), end(intf), [&macToFind](Arena::InterfaceInfo intf_temp)
	 * 		{
	 * 			return Intf_temp.MacAddressStr() == macToFind;
	 * 		});
	 * 	}
	 * \endcode
	 *
	 * @see 
	 *  - Arena::ISystem
	 *  - Arena::InterfaceInfo
	 */
	class ARENA_API InterfaceInfo
	{
	public:
		/**
		 * @fn virtual uint32_t IpAddress()
		 *
		 * @return 
		 *  - Type: uint32_t
		 *  - IP address of the interface as an integer
		 *
		 * <B> IpAddress </B> gets the IP address of an interface on the host,
		 * returning it as its raw integer value.
		 *
		 * Both <B> IpAddress </B> and <B> IpAddressStr </B> represent the same
		 * IP address. Where <B> IpAddress </B> returns the value in its raw
		 * integer format, <B> IpAddressStr </B> returns it as a more
		 * human-readable string:
		 *  - <B> IpAddress </B>: 0xA9FE0101 
		 *  - <B> IpAddressStr </B>: 169.254.1.1
		 * 
		 * An interface may have its IP address, subnet mask, and MAC Address
		 * checked through the main node map
		 * (Arena::ISystem::GetTLInterfaceNodeMap, 'GevInterfaceIPAddress',
		 * 'GevInterfaceSubnetMask', 'GevInterfaceMACAddress')
		 * 
		 * @warning 
		 *  - Represents same information as Arena::InterfaceInfo::IpAddressStr
		 *
		 * @see 
		 *  - Arena::ISystem::GetInterfaces
		 *  - Arena::ISystem::GetTLInterfaceNodeMap
		 *  - Arena::InterfaceInfo::IpAddressStr
		 */
		virtual uint32_t IpAddress();

		/**
		 * @fn virtual GenICam::gcstring IpAddressStr()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - IP address of the interface as a string
		 *
		 * <B> IpAddressStr </B> gets the IP address of an interface on the host,
		 * returning it as a string.
		 *
		 * Both <B> IpAddress </B> and <B> IpAddressStr </B> represent the same
		 * IP address. Where <B> IpAddress </B> returns the value in its raw
		 * integer format, <B> IpAddressStr </B> returns it as a more
		 * human-readable string:
		 *  - <B> IpAddress </B>: 0xA9FE0101 
		 *  - <B> IpAddressStr </B>: 169.254.1.1
		 *
		 * @warning 
		 *  - Represents same information as Arena::InterfaceInfo::IpAddress
		 * 
		 *
		 * @see 
		 *  - Arena::ISystem::GetInterfaces
		 *  - Arena::ISystem::GetTLInterfaceNodeMap
		 *  - Arena::InterfaceInfo::IpAddress
		 */
		virtual GenICam::gcstring IpAddressStr();

		/**
		 * @fn virtual uint32_t SubnetMask()
		 *
		 * @return 
		 *  - Type: uint32_t
		 *  - Subnet mask of the interface as an integer
		 *
		 * <B> SubnetMask </B> gets the subnet mask of an interface on the host,
		 * returning it as its raw integer value.
		 *
		 * Both <B> SubnetMask </B> and <B> SubnetMaskStr </B> represent the same
		 * subnet mask. Where <B> SubnetMask </B> returns the value in its raw
		 * integer format, <B> SubnetMaskStr </B> returns it as a more
		 * human-readable string:
		 *  - <B> SubnetMask </B>: 0xFFFF0000 
		 *  - <B> SubnetMaskStr </B>: 255.255.0.0
		 *
		 * An interface may have its IP address, subnet mask, and MAC Address
		 * checked through the main node map
		 * (Arena::ISystem::GetTLInterfaceNodeMap, 'GevInterfaceIPAddress',
		 * 'GevInterfaceSubnetMask', 'GevInterfaceMACAddress').
		 *
		 * @warning 
		 *  - Represents same information as
		 *    Arena::InterfaceInfo::SubnetMaskStr
		 *
		 * @see 
		 *  - Arena::ISystem::GetInterfaces
		 *  - Arena::ISystem::GetTLInterfaceNodeMap
		 *  - Arena::InterfaceInfo::SubnetMaskStr
		 */
		virtual uint32_t SubnetMask();

		/**
		 * @fn virtual GenICam::gcstring SubnetMaskStr()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - Subnet mask of the device as a string
		 *
		 * <B> SubnetMaskStr </B> gets the subnet mask of an interface on the
		 * host, returning it as a string.
		 *
		 * Both <B> SubnetMask </B> and <B> SubnetMaskStr </B> represent the same
		 * subnet mask. Where <B> SubnetMask </B> returns the value in its raw
		 * integer format, <B> SubnetMaskStr </B> returns it as a more
		 * human-readable string:
		 *  - <B> SubnetMask </B>: 0xFFFF0000 
		 *  - <B> SubnetMaskStr </B>: 255.255.0.0
		 *
		 * @warning 
		 *  - Represents same information as Arena::InterfaceInfo::SubnetMask
		 *
		 * @see 
		 *  - Arena::ISystem::GetInterfaces
		 *  - Arena::ISystem::GetTLInterfaceNodeMap
		 *  - Arena::InterfaceInfo::SubnetMask
		 */
		virtual GenICam::gcstring SubnetMaskStr();

		/**
		 * @fn virtual uint64_t MacAddress()
		 *
		 * @return 
		 *  - Type: uint64_t
		 *  - MAC address of the interface as an integer
		 *
		 * <B> MacAddress </B> gets the MAC address of an interface on the host,
		 * returning it as its raw integer value.
		 *
		 * Both <B> MacAddress </B> and <B> MacAddressStr </B> represent the same
		 * MAC address. Where <B> MacAddress </B> returns the value in its raw
		 * integer format, <B> MacAddressStr </B> returns it as a more
		 * human-readable string:
		 *  - <B> MacAddress </B>: 0x1C0FAF010101 
		 *  - <B> MacAddressStr </B>: 1C:0F:AF:01:01:01
		 *
		 * @warning 
		 *  - Represents same information as
		 *    Arena::InterfaceInfo::MacAddressStr
		 *
		 * @see 
		 *  - Arena::ISystem::GetInterfaces
		 *  - Arena::ISystem::GetTLInterfaceNodeMap
		 *  - Arena::InterfaceInfo::MacAddressStr
		 */
		virtual uint64_t MacAddress();

		/**
		 * @fn virtual GenICam::gcstring MacAddressStr()
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - MAC address of the interface as a string
		 *
		 * <B> MacAddressStr </B> gets the MAC address of an interface on the
		 * host, returning it as a string.
		 *
		 * Both <B> MacAddress </B> and <B> MacAddressStr </B> represent the same
		 * MAC address. Where <B> MacAddress </B> returns the value in its raw
		 * integer format, <B> MacAddressStr </B> returns it as a more
		 * human-readable string:
		 *  - <B> MacAddress </B>: 0x1C0FAF010101 
		 *  - <B> MacAddressStr </B>: 1C:0F:AF:01:01:01
		 *
		 * @warning 
		 *  - Represents same information as Arena::InterfaceInfo::MacAddress
		 *
		 * @see 
		 *  - Arena::ISystem::GetInterfaces
		 *  - Arena::ISystem::GetTLInterfaceNodeMap
		 *  - Arena::InterfaceInfo::MacAddress
		 */
		virtual GenICam::gcstring MacAddressStr();

		/**
		 * @fn InterfaceInfo()
		 *
		 * An empty constructor
		 *
		 * @warning 
		 *  - Does not sufficiently initialize InterfaceInfo
		 */
		InterfaceInfo();

		/**
		 * @fn InterfaceInfo(const InterfaceInfo& deviceInfo)
		 *
		 * A copy constructor
		 *
		 * @param deviceInfo
		 *  - Type: const InterfaceInfo&
		 *  - Device information object to copy
		 */
		InterfaceInfo(const InterfaceInfo& deviceInfo);

		/**
		 * @fn virtual ~InterfaceInfo()
		 *
		 * A destructor
		 */
		virtual ~InterfaceInfo();

		/**
		 * @fn virtual InterfaceInfo& operator=(InterfaceInfo deviceInfo)
		 *
		 * A copy assignment operator
		 *
		 * @param deviceInfo
		 *  - Type: InterfaceInfo
		 *  - Device information object to copy
		 *
		 * @return 
		 *  - Type: InterfaceInfo&
		 *  - Copied device information object
		 */
		virtual InterfaceInfo& operator=(InterfaceInfo deviceInfo);

	protected:
		friend class System;
		void* m_pData;
	};
} // namespace Arena

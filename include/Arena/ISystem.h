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

#include "DeviceInfo.h"
#include "InterfaceInfo.h"
#include "IDevice.h"
#include <vector>

namespace Arena
{
	/**
	 * @class ISystem
	 *
	 * An interface to the system object
	 *
	 * The system is the entry point to the Arena SDK. It is retrieved and
	 * cleaned up through global functions (Arena::OpenSystem,
	 * Arena::CloseSystem).
	 *
	 * \code{.cpp}
	 * 	// opening and closing the system
	 * 	{
	 * 		Arena::ISystem* pSystem = Arena::OpenSystem();
	 * 		// do something
	 * 		// ...
	 * 		Arena::CloseSystem(pSystem);
	 * 	}
	 * \endcode
	 *
	 * It manages devices (Arena::DeviceInfo, Arena::IDevice) and the system node
	 * map (GenApi::INodeMap) by:
	 *  - maintaing a list of enumerated devices
	 *    (Arena::ISystem::UpdateDevices, Arena::ISystem::GetDevices),
	 *  - creating and destroying devices (Arena::ISystem::CreateDevice,
	 *    Arena::ISystem::DestroyDevice),
	 *  - and providing access to its node map
	 *    (Arena::ISystem::GetTLSystemNodeMap).
	 *
	 * @warning 
	 *  - May only be opened once; subsequent attempts will throw
	 *  - Must be closed as final step with Arena; otherwise, memory will leak
	 *
	 * @see 
	 *  - Arena::ISystem
	 *  - Arena::DeviceInfo
	 *  - Arena::IDevice
	 */
	class ARENA_API ISystem
	{
	public:

		/**
		 * @fn virtual std::vector<InterfaceInfo> GetInterfaces()
		 *
		 * @return 
		 *  - Type: std::vector<Arena::InterfaceInfo>
		 *  - Vector of interface information objects
		 *  - Each interface information object refers to am interface on the
		 *    system
		 *
		 * <B> GetInterfaces </B> retrieves the internally maintained list of
		 * interface information objects (Arena::InterfaceInfo).
		 *
		 * Internally, the system creates a list of information on all enumerated
		 * interfaces. <B> GetInterface </B> updates and retrieves this list. The
		 * interface information objects can be used to specify which interface
		 * to update devices on (Arena::ISystem::UpdateDevices).
		 *
		 * The device information objects (Arena::InterfaceInfo) in the returned
		 * list merely house information in order to differentiate one interface
		 * from another. A standard vector (std::vector) is returned so that it
		 * can be searched and iterated over using the C++ standard library.
		 *
		 * @warning 
		 *  - Returns an empty list if no interfaces discovered
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::InterfaceInfo
		 *  - Arena::ISystem::UpdateDevices
		 */
		virtual std::vector<InterfaceInfo> GetInterfaces() = 0;

		/**
		 * @fn virtual bool UpdateDevices(uint64_t timeout)
		 *
		 * @param timeout
		 *  - Type: uint64_t
		 *  - Unit: milliseconds
		 *  - Time to wait for connected devices to respond
		 *
		 * @return 
		 *  - Type: bool
		 *  - True on first call that a device is found
		 *  - True if the device list has changed since the last call
		 *  - Otherwise, false
		 *
		 * <B> UpdateDevices </B> updates the internal list of devices, (along
		 * with their relevant interfaces). It must be called before retrieving
		 * the list of devices (Arena::ISystem::GetDevices) or any time that an
		 * updated device list might be necessary.
		 *
		 * When called, the system broadcasts a discovery packet to all
		 * interfaces, waiting until the end of the timeout for any responses
		 * from enumerated devices. The new, updated list of devices is compared
		 * to the old list. If the contents of the list have changed, 'true' is
		 * returned; otherwise 'false'.
		 *
		 * The GigE Vision spec requires devices respond to a broadcast discovery
		 * packet within one second unless set otherwise
		 * (Arena::IDevice::GetNodeMap, 'DiscoveryAckDelay'). Lucid devices are
		 * set to respond within 100 ms. Therefore, 100 works as an appropriate
		 * timeout value in many use cases. This response time can be customized
		 * through the 'DiscoveryAckDelay' feature, if supported. The timeout
		 * value should reflect any such changes.
		 *
		 * @warning 
		 *  - Slightly affects bandwidth usage due to the broadcasting of
		 *    discovery packets
		 *  - Discovers devices on all subnets, even when unable to communicate
		 *    with them due to IP configuration
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::ISystem::GetDevices
		 *  - Arena::IDevice::GetNodeMap
		 */
		virtual bool UpdateDevices(uint64_t timeout) = 0;

		/**
		 * @fn virtual bool UpdateDevices(InterfaceInfo ifaceInfo, uint64_t timeout)
		 *
		 * @param ifaceInfo
		 *  - Type: InterfaceInfo
		 *  - Specific interface to enumerate devices on
		 *
		 * @param timeout
		 *  - Type: uint64_t
		 *  - Unit: milliseconds
		 *  - Time to wait for connected devices to respond
		 *
		 * @return 
		 *  - Type: bool
		 *  - True on first call that a device is found
		 *  - True if the device list has changed since the last call
		 *  - Otherwise, false
		 *
		 * <B> UpdateDevices </B> updates the internal list of devices, (along
		 * with their relevant interfaces). It must be called before retrieving
		 * the list of devices (Arena::ISystem::GetDevices) or any time that an
		 * updated device list might be necessary.
		 *
		 * When called, the system broadcasts a discovery packet to all
		 * interfaces, waiting until the end of the timeout for any responses
		 * from enumerated devices. The new, updated list of devices is compared
		 * to the old list. If the contents of the list have changed, 'true' is
		 * returned; otherwise 'false'.
		 *
		 * The GigE Vision spec requires devices respond to a broadcast discovery
		 * packet within one second unless set otherwise
		 * (Arena::IDevice::GetNodeMap, 'DiscoveryAckDelay'). Lucid devices are
		 * set to respond within 100 ms. Therefore, 100 works as an appropriate
		 * timeout value in many use cases. This response time can be customized
		 * through the 'DiscoveryAckDelay' feature, if supported. The timeout
		 * value should reflect any such changes.
		 *
		 * @warning 
		 *  - Slightly affects bandwidth usage due to the broadcasting of
		 *    discovery packets
		 *  - Discovers devices on all subnets, even when unable to communicate
		 *    with them due to IP configuration
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::ISystem::GetDevices
		 *  - Arena::IDevice::GetNodeMap
		 */
		virtual bool UpdateDevices(InterfaceInfo ifaceInfo, uint64_t timeout) = 0;

		/**
		 * @fn virtual std::vector<DeviceInfo> GetDevices()
		 *
		 * @return 
		 *  - Type: std::vector<Arena::DeviceInfo>
		 *  - Vector of device information objects
		 *  - Each device information object refers to a connected device
		 *
		 * <B> GetDevices </B> retrieves the internally maintained list of device
		 * information objects (Arena::DeviceInfo). It must be called after the
		 * list has been updated (Arena::ISystem::UpdateDevices) and before a
		 * device is created (Arena::ISystem::CreateDevice).
		 *
		 * Internally, the system stores a list of information on all enumerated
		 * devices. As a simple getter, <B> GetDevices </B> retrieves this list
		 * without doing anything to update, maintain, or manage it.
		 *
		 * The device information objects (Arena::DeviceInfo) in the returned
		 * list should not be confused with Arena::IDevice objects. Whereas the
		 * latter are handles used to interact with a physical device, device
		 * information objects merely house information in order to differentiate
		 * one from another. A standard vector (std::vector) is returned so that
		 * it can be searched and iterated over using conventional means of the
		 * C++ standard library.
		 *
		 * @warning 
		 *  - Returns an empty list if list never updated
		 *  - Returns objects containing device information, not device objects
		 *    themselves
		 *  - Returns devices on all subnets, even when unable to communicate
		 *    with them due to IP configuration
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::DeviceInfo
		 *  - Arena::IDevice
		 *  - Arena::ISystem::UpdateDevices
		 *  - Arena::ISystem::CreateDevice
		 */
		virtual std::vector<DeviceInfo> GetDevices() = 0;

		/**
		 * @fn virtual IDevice* CreateDevice(DeviceInfo info)
		 *
		 * @param info
		 *  - Type: Arena::DeviceInfo
		 *  - Device information object of the device to create
		 *
		 * @return 
		 *  - Type: Arena::IDevice*
		 *  - Pointer to an initialized, ready-to-use device
		 *
		 * <B> CreateDevice </B> creates and initializes a device using a single
		 * device information object (Arena::DeviceInfo). It must be called after
		 * devices have been retrieved (Arena::ISystem::GetDevices). The device
		 * must be destroyed (Arena::ISystem::DestroyDevice) when no longer
		 * needed.
		 *
		 * When called, <B> CreateDevice </B> prepares the camera for user
		 * interaction, opening the control channel socket and initializing all
		 * node maps (GenApi::INodeMap). The returned device is ready to stream
		 * images, send events, and read or customize features.
		 *
		 * A single process may only create a single device once, but a single
		 * device may be opened on multiple processes. The first process to
		 * create the device is given read-write access. Additional processes are
		 * given read-only access. With read-only access, processes can read
		 * features and receive images and events; they cannot, however, write
		 * values, start the image stream or initialize events.
		 *
		 * @warning 
		 *  - Provides read-write access only to initial process to create
		 *    device; following processes given read-only access
		 *  - Devices must be destroyed
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::DeviceInfo
		 *  - Arena::IDevice
		 *  - Arena::ISystem::GetDevices
		 *  - Arena::ISystem::DestroyDevice
		 */
		virtual IDevice* CreateDevice(DeviceInfo info) = 0;

		/**
		 * @fn virtual void DestroyDevice(IDevice* pDevice)
		 *
		 * @param pDevice
		 *  - Type: Arena::IDevice*
		 *  - Device to destroy
		 *
		 * @return 
		 *  - none
		 *
		 * <B> DestroyDevice </B> destroys and cleans up the internal memory of a
		 * device (Arena::IDevice). Devices that have been created
		 * (Arena::ISystem::CreateDevice) must be destroyed.
		 *
		 * When called, <B> DestroyDevice </B> deletes all internal memory
		 * associated with a device: if a stream has been left open, it is
		 * closed; all node maps and chunk data adapters are deallocated; events
		 * are unregistered and the message channel closed; finally, the control
		 * channel socket is closed, allowing the device to be opened in
		 * read-write mode again.
		 *
		 * Destroying a device does not reset device settings, and will not
		 * return a camera to a stable state. To reset settings or return to a
		 * stable state, power-cycle a device (unplug and plug back in) or reset
		 * it ('DeviceReset' feature).
		 *
		 * @warning 
		 *  - Devices must be destroyed
		 *  - Does not affect device settings
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::IDevice
		 *  - Arena::ISystem::CreateDevice
		 */
		virtual void DestroyDevice(IDevice* pDevice) = 0;

		/**
		 * @fn virtual GenApi::INodeMap* GetTLSystemNodeMap()
		 *
		 * @return 
		 *  - Type: GenApi::INodeMap*
		 *  - GenTL node map for the system
		 *
		 * <B> GetTLSystemNodeMap </B> retrieves the GenTL system node map
		 * (GenApi::INodeMap), used to access system related nodes (GenApi::INode).
		 *
		 * As a simple getter, <B> GetTLSystemNodeMap </B> retrieves this node
		 * map without doing anything to initialize, manage, or maintain it. This
		 * node map is initialized when the system is opened (Arena::OpenSystem)
		 * and deinitialized when the system is closed (Arena::CloseSystem).
		 * Because node maps are cleaned up internally, retrieving multiple
		 * pointers to the same node map is permitted.
		 *
		 * All available nodes can be viewed in ArenaView or the examples
		 * (Cpp_Explore_NodeMaps) example. Nodes in this node map include nodes
		 * related to:
		 *  - Arena SDK information
		 *  - GenTL and GEV versioning information
		 *  - the ability to update and select interfaces
		 *  - interface discovery and IP configuration information
		 *
		 * Arena provides access to five different node maps. This one comes from
		 * the device and describes all its features. Please check device
		 * documentation for more information regarding these features.
		 *  - device (Arena::IDevice::GetNodeMap)
		 *
		 * The other four, including this one, node maps describe and provide
		 * access to information and settings through the software rather than
		 * the device.
		 *  - system GenTL (Arena::ISystem::GetTLSystemNodeMap)
		 *  - stream GenTL (Arena::IDevice::GetTLStreamNodeMap)
		 *  - device GenTL (Arena::IDevice::GetTLDeviceNodeMap)
		 *  - interface GenTL (Arena::IDevice::GetTLInterfaceNodeMap)
		 *
		 * @warning 
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::OpenSystem
		 *  - Arena::CloseSystem
		 *  - Arena::IDevice::GetNodeMap
		 *  - Arena::ISystem::GetTLSystemNodeMap
		 *  - Arena::IDevice::GetTLStreamNodeMap
		 *  - Arena::IDevice::GetTLDeviceNodeMap
		 *  - Arena::IDevice::GetTLInterfaceNodeMap
		 */
		virtual GenApi::INodeMap* GetTLSystemNodeMap() = 0;

		/**
		 * @fn virtual GenApi::INodeMap* GetTLInterfaceNodeMap(DeviceInfo devInfo)
		 *
		 * @param devInfo
		 *  - Type: DeviceInfo
		 *  - Device to get the interface nodemap for
		 *
		 * @return 
		 *  - Type: GenApi::INodeMap*
		 *  - GenTL node map for the system
		 *
		 * <B> GetTLInterfaceNodeMap </B> retrieves the GenTL Interface node map
		 * (GenApi::INodeMap), used to access interface related nodes
		 * (GenApi::INode).
		 *
		 * As a simple getter, <B> GetTLInterfaceNodeMap </B> retrieves this node
		 * map without doing anything to initialize, manage, or maintain it. This
		 * node map is initialized when the system is opened (Arena::OpenSystem)
		 * and deinitialized when the system is closed (Arena::CloseSystem).
		 * Because node maps are cleaned up internally, retrieving multiple
		 * pointers to the same node map is permitted. This nodemap will be
		 * associated to the device indicated by the devInfo (Arena::DeviceInfo)
		 * parameter.
		 *
		 * All available nodes can be viewed in the XML,
		 * SFNC_GenTLInterface_Reference_Version_1_0_0_Schema_1_1.xml, found in
		 * Arena/<version>/<platform>/xml. Nodes in this node map include nodes
		 * related to:
		 *  - Interface information
		 *  - GenTL and GEV versioning information
		 *  - the ability to update and select devices
		 *  - device discovery and IP configuration information
		 *
		 * Arena provides access to five different node maps. This one comes from
		 * the device and describes all its features. Please check device
		 * documentation for more information regarding these features.
		 *  - device (Arena::IDevice::GetNodeMap)
		 *
		 * The other four, including this one, node maps describe and provide
		 * access to information and settings through the software rather than
		 * the device.
		 *  - system GenTL (Arena::ISystem::GetTLSystemNodeMap)
		 *  - stream GenTL (Arena::IDevice::GetTLStreamNodeMap)
		 *  - device GenTL (Arena::IDevice::GetTLDeviceNodeMap)
		 *  - interface GenTL (Arena::IDevice::GetTLInterfaceNodeMap)
		 *
		 * @warning 
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::DeviceInfo
		 *  - Arena::OpenSystem
		 *  - Arena::CloseSystem
		 *  - Arena::IDevice::GetNodeMap
		 *  - Arena::ISystem::GetTLSystemNodeMap
		 *  - Arena::IDevice::GetTLStreamNodeMap
		 *  - Arena::IDevice::GetTLDeviceNodeMap
		 *  - Arena::IDevice::GetTLInterfaceNodeMap
		 */
		virtual GenApi::INodeMap* GetTLInterfaceNodeMap(DeviceInfo devInfo) = 0;

		/**
		 * @fn virtual void ForceIp(uint64_t macAddress, uint64_t ipAddress, uint64_t subnetMask, uint64_t defaultGateway)
		 *
		 * @param macAddress a 48 bit mac address identifying the device to force the
		 * ip to.
		 *  - ipAddress a 32bit integer representing the IP address that is
		 *    meant to be forced to the device
		 *  - subnetMask a 32bit integer representing the subnetmask that is
		 *    meant to be forced to the device
		 *  - defaultGateway a 32 bit integer representing the gateway that is
		 *    meant to be forced to the device
		 *
		 * Forces the device that matches the macAddress to a temporary new
		 * ipAddress, subnetMask and defaultGateway
		 *
		 * Arena::ISystem::ForceIp will send a ForceIP command out on all the
		 * interfaces This call also updates the internal list of interfaces in
		 * case that has not been done yet. The ForceIP command will be a network
		 * wide broadcast 255.255.255.255 and will request an acknowledment to be
		 * broadcast back to the host.
		 *
		 * @warning 
		 *  - This function may throw an exception derived from
		 *    GenApi::GenericExcpetion upon failure
		 *
		 * @see 
		 *  - Arena::ISystem::ForceIp
		 */
		virtual void ForceIp(uint64_t macAddress, uint64_t ipAddress, uint64_t subnetMask, uint64_t defaultGateway) = 0;

		/**
		 * @fn virtual ~ISystem()
		 *
		 * A destructor
		 */
		virtual ~ISystem(){};

	protected:

		// empty ctor
		// inaccessible
		ISystem(){};
	};
} // namespace Arena

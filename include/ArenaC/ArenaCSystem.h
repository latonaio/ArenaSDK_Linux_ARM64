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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn AC_ERROR AC_API acSystemGetNumInterfaces(acSystem hSystem, size_t* pNumDevices)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param pNumDevices
 *  - Type: size_t*
 *  - [Out] parameter
 *  - The number of discovered interfaces
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetNumInterfaces </B> retrieves the number of discovered
 * interfaces.
 */
AC_ERROR AC_API acSystemGetNumInterfaces(acSystem hSystem, size_t* pNumDevices);

/**
 * @fn AC_ERROR AC_API acSystemGetInterfaceIpAddress(acSystem hSystem, size_t index, uint32_t* pIpAddress)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the interface
 *
 * @param pIpAddress
 *  - Type: uint32_t*
 *  - [Out] parameter
 *  - IP address as an integer
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetInterfaceIpAddress </B> gets the IP address of a interface on
 * the network, returning it as its raw integer value.
 */
AC_ERROR AC_API acSystemGetInterfaceIpAddress(acSystem hSystem, size_t index, uint32_t* pIpAddress);

/**
 * @fn AC_ERROR AC_API acSystemGetInterfaceIpAddressStr(acSystem hSystem, size_t index, char* pIpAddressStr, size_t* pBufLen)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the interface
 *
 * @param pIpAddressStr
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - IP address as a dot-separated string
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetInterfaceIpAddressStr </B> gets the IP address of a interface
 * on the network, returning it as a string.
 */
AC_ERROR AC_API acSystemGetInterfaceIpAddressStr(acSystem hSystem, size_t index, char* pIpAddressStr, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemGetInterfaceSubnetMask(acSystem hSystem, size_t index, uint32_t* pSubnetMask)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the interface
 *
 * @param pSubnetMask
 *  - Type: uint32_t*
 *  - [Out] parameter
 *  - Subnet mask as an integer
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetInterfaceSubnetMask </B> gets the subnet mask of an interface
 * on the network, returning it as its raw integer value.
 */
AC_ERROR AC_API acSystemGetInterfaceSubnetMask(acSystem hSystem, size_t index, uint32_t* pSubnetMask);

/**
 * @fn AC_ERROR AC_API acSystemGetInterfaceSubnetMaskStr(acSystem hSystem, size_t index, char* pSubnetMaskStr, size_t* pBufLen)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the interface
 *
 * @param pSubnetMaskStr
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Subnet mask as a dot-separated string
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetInterfaceSubnetMaskStr </B> gets the subnet mask of an
 * interface on the network, returning it as a string.
 */
AC_ERROR AC_API acSystemGetInterfaceSubnetMaskStr(acSystem hSystem, size_t index, char* pSubnetMaskStr, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemGetInterfaceMacAddress(acSystem hSystem, size_t index, uint64_t* pMacAddress)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - MAC address as an integer
 *
 * @param pMacAddress
 *  - Type: uint64_t*
 *  - [Out] parameter
 *  - MAC address
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetInterfaceMacAddress </B> gets the MAC address of a interface on
 * the network, returning it as its raw integer value.
 */
AC_ERROR AC_API acSystemGetInterfaceMacAddress(acSystem hSystem, size_t index, uint64_t* pMacAddress);

/**
 * @fn AC_ERROR AC_API acSystemGetInterfaceMacAddressStr(acSystem hSystem, size_t index, char* pMacAddress, size_t* pBufLen)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the interface
 *
 * @param pMacAddress
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - MAC address as a colon-separated string
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetInterfaceMacAddressStr </B> gets the MAC address of a interface
 * on the network, returning it as a string.
 */
AC_ERROR AC_API acSystemGetInterfaceMacAddressStr(acSystem hSystem, size_t index, char* pMacAddress, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemUpdateDevices(acSystem hSystem, uint64_t timeout)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param timeout
 *  - Type: uint64_t
 *  - [In] parameter
 *  - Time to wait for connected devices to respond
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemUpdateDevices </B> updates the internal list of devices, (along
 * with their relevant interfaces). It must be called before retrieving the
 * number of devices (acSystemGetNumDevices) or any time that an updated device
 * list might be necessary.
 *
 * @see 
 *  - acSystemGetNumDevices
 */
AC_ERROR AC_API acSystemUpdateDevices(acSystem hSystem, uint64_t timeout);

/**
 * @fn AC_ERROR AC_API acSystemUpdateDevicesHasChanged(acSystem hSystem, uint64_t timeout, bool8_t* pHasChanged)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param timeout
 *  - Type: uint64_t
 *  - [In] parameter
 *  - Time to wait for connected devices to respond
 *
 * @param pHasChanged
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True on first call that a device is found
 *  - True if the device list has changed since the last call
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemUpdateDevicesHasChanged </B> updates the internal list of devices,
 * (along with their relevant interfaces). It must be called before retrieving
 * the number of devices (acSystemGetNumDevices) or any time that an updated
 * device list might be necessary.
 *
 * @see 
 *  - acSystemGetNumDevices
 */
AC_ERROR AC_API acSystemUpdateDevicesHasChanged(acSystem hSystem, uint64_t timeout, bool8_t* pHasChanged);

/**
 * @fn AC_ERROR AC_API acSystemUpdateDevicesOnInterface(acSystem hSystem, size_t interfaceIndex, uint64_t timeout, bool8_t* pHasChanged)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param interfaceIndex
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the interface
 *
 * @param timeout
 *  - Type: uint64_t
 *  - [In] parameter
 *  - Time to wait for connected devices to respond
 *
 * @param pHasChanged
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True on first call that a device is found
 *  - True if the device list has changed since the last call
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemUpdateDevicesOnInterface </B> updates the internal list of
 * devices, (along with their relevant interfaces). It must be called before
 * retrieving the number of devices (acSystemGetNumDevices) or any time that an
 * updated device list might be necessary.
 *
 * @see 
 *  - acSystemGetNumDevices
 */
AC_ERROR AC_API acSystemUpdateDevicesOnInterface(acSystem hSystem, size_t interfaceIndex, uint64_t timeout, bool8_t* pHasChanged);

/**
 * @fn AC_ERROR AC_API acSystemGetNumDevices(acSystem hSystem, size_t* pNumDevices)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param pNumDevices
 *  - Type: size_t*
 *  - [Out] parameter
 *  - The number of discovered devices
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetNumDevices </B> retrieves the number of discovered devices. It
 * must be called after updating the internal list of devices
 * (acSystemUpdateDevices).
 *
 * @see 
 *  - acSystemUpdateDevices
 */
AC_ERROR AC_API acSystemGetNumDevices(acSystem hSystem, size_t* pNumDevices);

/**
 * @fn AC_ERROR AC_API acSystemGetDeviceModel(acSystem hSystem, size_t index, char* pModelNameBuf, size_t* pBufLen)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pModelNameBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Model name of the device
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetDeviceModel </B> gets the model name of a device.
 */
AC_ERROR AC_API acSystemGetDeviceModel(acSystem hSystem, size_t index, char* pModelNameBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemGetDeviceVendor(acSystem hSystem, size_t index, char* pVendorNameBuf, size_t* pBufLen)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pVendorNameBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Vendor name of the device
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetDeviceVendor </B> gets the vendor/manufacturer name of a
 * device. Vendor names differentiate between device vendors/manufacturers. Lucid
 * devices return 'Lucid Vision Labs'.
 */
AC_ERROR AC_API acSystemGetDeviceVendor(acSystem hSystem, size_t index, char* pVendorNameBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemGetDeviceSerial(acSystem hSystem, size_t index, char* pSerialNumberBuf, size_t* pBufLen)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pSerialNumberBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Serial number of the device
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acDeviceGetDeviceSerial </B> gets the serial number of a device. A serial
 * number differentiates between devices. Each Lucid device has a unique serial
 * number. Lucid serial numbers are numeric, but the serial numbers of other
 * vendors may be alphanumeric.
 */
AC_ERROR AC_API acSystemGetDeviceSerial(acSystem hSystem, size_t index, char* pSerialNumberBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemGetDeviceIpAddress(acSystem hSystem, size_t index, uint32_t* pIpAddress)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pIpAddress
 *  - Type: uint32_t*
 *  - [Out] parameter
 *  - IP address as an integer
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetDeviceIpAddress </B> gets the IP address of a device on the
 * network, returning it as its raw integer value.
 */
AC_ERROR AC_API acSystemGetDeviceIpAddress(acSystem hSystem, size_t index, uint32_t* pIpAddress);

/**
 * @fn AC_ERROR AC_API acSystemGetDeviceIpAddressStr(acSystem hSystem, size_t index, char* pIpAddressStr, size_t* pBufLen)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pIpAddressStr
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - IP address as a dot-separated string
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetDeviceIpAddressStr </B> gets the IP address of a device on the
 * network, returning it as a string.
 */
AC_ERROR AC_API acSystemGetDeviceIpAddressStr(acSystem hSystem, size_t index, char* pIpAddressStr, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemGetDeviceSubnetMask(acSystem hSystem, size_t index, uint32_t* pSubnetMask)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pSubnetMask
 *  - Type: uint32_t*
 *  - [Out] parameter
 *  - Subnet mask as an integer
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetDeviceSubnetMask </B> gets the subnet mask of a device on the
 * network, returning it as its raw integer value.
 */
AC_ERROR AC_API acSystemGetDeviceSubnetMask(acSystem hSystem, size_t index, uint32_t* pSubnetMask);

/**
 * @fn AC_ERROR AC_API acSystemGetDeviceSubnetMaskStr(acSystem hSystem, size_t index, char* pSubnetMaskStr, size_t* pBufLen)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pSubnetMaskStr
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Subnet mask as a dot-separated string
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetDeviceSubnetMaskStr </B> gets the subnet mask of a device on
 * the network, returning it as a string.
 */
AC_ERROR AC_API acSystemGetDeviceSubnetMaskStr(acSystem hSystem, size_t index, char* pSubnetMaskStr, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemGetDeviceDefaultGateway(acSystem hSystem, size_t index, uint32_t* pDefaultGateway)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pDefaultGateway
 *  - Type: uint32_t*
 *  - [Out] parameter
 *  - Default gateway as an integer
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 */
AC_ERROR AC_API acSystemGetDeviceDefaultGateway(acSystem hSystem, size_t index, uint32_t* pDefaultGateway);

/**
 * @fn AC_ERROR AC_API acSystemGetDeviceDefaultGatewayStr(acSystem hSystem, size_t index, char* pDefaultGatewayStr, size_t* pBufLen)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pDefaultGatewayStr
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Default gateway as a dot-separated string
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 */
AC_ERROR AC_API acSystemGetDeviceDefaultGatewayStr(acSystem hSystem, size_t index, char* pDefaultGatewayStr, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemGetDeviceMacAddress(acSystem hSystem, size_t index, uint64_t* pMacAddress)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - MAC address as an integer
 *
 * @param pMacAddress
 *  - Type: uint64_t*
 *  - [Out] parameter
 *  - MAC address
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetDeviceMacAddress </B> gets the MAC address of a device on the
 * network, returning it as its raw integer value.
 */
AC_ERROR AC_API acSystemGetDeviceMacAddress(acSystem hSystem, size_t index, uint64_t* pMacAddress);

/**
 * @fn AC_ERROR AC_API acSystemGetDeviceMacAddressStr(acSystem hSystem, size_t index, char* pMacAddress, size_t* pBufLen)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pMacAddress
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - MAC address as a colon-separated string
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemGetDeviceMacAddressStr </B> gets the MAC address of a device on
 * the network, returning it as a string.
 */
AC_ERROR AC_API acSystemGetDeviceMacAddressStr(acSystem hSystem, size_t index, char* pMacAddress, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemForceIpAddress(acSystem hSystem, uint64_t macAddress, uint64_t ipAddress, uint64_t subnetMask, uint64_t defaultGateway)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param macAddress
 *  - Type: uint64_t
 *  - [In] parameter
 *  - MAC address
 *
 * @param ipAddress
 *  - Type: uint64_t
 *  - [In] parameter
 *  - Ip address
 *
 * @param subnetMask
 *  - Type: uint64_t
 *  - [In] parameter
 *  - subnetMask address
 *
 * @param defaultGateway
 *  - Type: uint64_t
 *  - [In] parameter
 *  - defaultGateway address
 *
 * @return
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemForceIpAddress </B> force sets the IP address of a device on the
 * network
 */
AC_ERROR AC_API acSystemForceIpAddress(acSystem hSystem, uint64_t macAddress, uint64_t ipAddress, uint64_t subnetMask, uint64_t defaultGateway);

/**
* @fn AC_ERROR AC_API acSystemGetDeviceUserDefinedName(acSystem hSystem, size_t index, char* pUserDefinedName, size_t* pBufLen)
*
* @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
*
* @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
*
* @param pUserDefinedName
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - User-defined name
*
* @param pBufLen
 *  - Type: size_t*
 *  - [In/out] parameter
*  - (In) Length of the buffer
*  - (Out) Length of the value
*
* @return
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
*
 * <B> acSystemGetDeviceUserDefinedName </B> gets the user-defined name of a
 * device. If supported, it is a customizable string with a maximum of 16 bytes
 * that can be used to identify a device.
*/
AC_ERROR AC_API acSystemGetDeviceUserDefinedName(acSystem hSystem, size_t index, char* pUserDefinedName, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemIsDeviceDHCPConfigurationEnabled(acSystem hSystem, size_t index, bool8_t* pIsDHCPConfigurationEnabled)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pIsDHCPConfigurationEnabled
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if DHCP enabled on device
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 */
AC_ERROR AC_API acSystemIsDeviceDHCPConfigurationEnabled(acSystem hSystem, size_t index, bool8_t* pIsDHCPConfigurationEnabled);

/**
 * @fn AC_ERROR AC_API acSystemIsDevicePersistentIpConfigurationEnabled(acSystem hSystem, size_t index, bool8_t* pIsPersistentIpConfigurationEnabled)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pIsPersistentIpConfigurationEnabled
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if persisitent IP configuration set on device
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 */
AC_ERROR AC_API acSystemIsDevicePersistentIpConfigurationEnabled(acSystem hSystem, size_t index, bool8_t* pIsPersistentIpConfigurationEnabled);

/*
 * @fn AC_ERROR AC_API acSystemGetDeviceVersion(acSystem hSystem, size_t index, char* pDeviceVersion, size_t* pBufLen)
 *
 * @param hSystem
 *	- Type: acSystem
 *	- [In] parameter
 *	- The system object
 *
 * @param index
 *	- Type: size_t
 *	- [In] parameter
 *	- Index of the device
 *
 * @param pDeviceVersion
 * - Type: char* 
 * - [Out] parameter 
 * - Accepts null 
 * - Device version
 *
 * @param pBufLen
 * - Type: size_t* 
 * - [In/out] parameter 
 * - (In) Length of the buffer 
 * - (Out) Length of the value
 */
AC_ERROR AC_API acSystemGetDeviceVersion(acSystem hSystem, size_t index, char* pDeviceVersion, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acSystemIsDeviceLLAConfigurationEnabled(acSystem hSystem, size_t index, bool8_t* pIsLLAIpConfigurationEnabled)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param pIsLLAIpConfigurationEnabled
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if LLA enabled on device
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 */
AC_ERROR AC_API acSystemIsDeviceLLAConfigurationEnabled(acSystem hSystem, size_t index, bool8_t* pIsLLAIpConfigurationEnabled);

/**
 * @fn AC_ERROR AC_API acSystemCreateDevice(acSystem hSystem, size_t index, acDevice* phDevice)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the device
 *
 * @param phDevice
 *  - Type: acDevice*
 *  - [Out] parameter
 *  - Initialized, ready-to-use device
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemCreateDevice </B> creates and initializes a device. It must be
 * called after the device list has been updated (acSystemUpdateDevices). The
 * device must be destroyed (acSystemDestroyDevice) when no longer needed.
 *
 * @see 
 *  - acSystemUpdateDevices
 *  - acSystemDestroyDevice
 */
AC_ERROR AC_API acSystemCreateDevice(acSystem hSystem, size_t index, acDevice* phDevice);

/**
 * @fn AC_ERROR AC_API acSystemDestroyDevice(acSystem hSystem, acDevice hDevice)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param hDevice
 *  - Type: acDevice
 *  - [In] parameter
 *  - Device to destroy
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSystemDestroyDevice </B> destroys and cleans up the internal memory of a
 * device (acDevice). Devices that have been created (acSystemCreateDevice) must
 * be destroyed.
 *
 * @see 
 *  - acDevice
 *  - acSystemCreateDevice
 */
AC_ERROR AC_API acSystemDestroyDevice(acSystem hSystem, acDevice hDevice);

/**
 * @fn AC_ERROR AC_API acSystemGetTLSystemNodeMap(acSystem hSystem, acNodeMap* phNodeMap)
 *
 * @param hSystem
 *  - Type: acSystem
 *  - [In] parameter
 *  - The system object
 *
 * @param phNodeMap
 *  - Type: acNodeMap*
 *  - [Out] parameter
 *  - GenTL node map for the system
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> GetTLSystemNodeMap </B> retrieves the GenTL system node map (acNodeMap),
 * used to access system related nodes (acNode).
 *
 * @see 
 *  - acNodeMap
 *  - acNode
 */
AC_ERROR AC_API acSystemGetTLSystemNodeMap(acSystem hSystem, acNodeMap* phNodeMap);

#ifdef __cplusplus
} // extern "C"
#endif

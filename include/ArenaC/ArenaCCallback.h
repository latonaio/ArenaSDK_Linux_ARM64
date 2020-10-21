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
 * @fn AC_ERROR AC_API acCallbackRegister(acCallback* phCallback, acNode hNode, acCallbackFunction callbackFunction, void* pUserData)
 *
 * @param phCallback
 *  - Type: acCallback*
 *  - [Out] parameter
 *  - The callback handle
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node to set the callback on
 *
 * @param callbackFunction
 *  - Type: acCallbackFunction
 *  - [In] parameter
 *  - The function to call
 *
 * @param pUserData
 *  - Type: void*
 *  - [Out] parameter
 *  - Accepts null
 *  - User data available within the callback function
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acCallbackRegister </B> registers a callback on a node. The callback
 * handle (acCallback) should be stored in order to deregister the callback
 * later.
 *
 * @see 
 *  - acCallback
 */
AC_ERROR AC_API acCallbackRegister(acCallback* phCallback, acNode hNode, acCallbackFunction callbackFunction, void* pUserData);

/**
 * @fn AC_ERROR AC_API acCallbackDeregister(acCallback hCallback)
 *
 * @param hCallback
 *  - Type: acCallback
 *  - [In] parameter
 *  - The callback handle
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acCallbackDeregister </B> deregisters the callback.
 */
AC_ERROR AC_API acCallbackDeregister(acCallback hCallback);

#ifdef __cplusplus
} // extern "C"
#endif

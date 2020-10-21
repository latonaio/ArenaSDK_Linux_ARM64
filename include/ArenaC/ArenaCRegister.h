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
 * @fn AC_ERROR AC_API acRegisterSet(acNode hNode, const uint8_t* pBuf, int64_t bufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts register nodes
 *
 * @param pBuf
 *  - Type: const uint8_t*
 *  - [In] parameter
 *  - Buffer with the value to set
 *
 * @param bufLen
 *  - Type: int64_t
 *  - [In] parameter
 *  - Length of buffer to set
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acRegisterSet </B> sets a register node's value.
 */
AC_ERROR AC_API acRegisterSet(acNode hNode, const uint8_t* pBuf, int64_t bufLen);

/**
 * @fn AC_ERROR AC_API acRegisterGet(acNode hNode, uint8_t* pBuf, int64_t bufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts register nodes
 *
 * @param pBuf
 *  - Type: uint8_t*
 *  - [Out] parameter
 *  - Empty buffer to get the value
 *
 * @param bufLen
 *  - Type: int64_t
 *  - [In] parameter
 *  - Length of the buffer to get
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acRegisterGet </B> gets a register node's value.
 */
AC_ERROR AC_API acRegisterGet(acNode hNode, uint8_t* pBuf, int64_t bufLen);

#ifdef __cplusplus
} // extern "C"
#endif

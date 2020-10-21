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
 * @fn AC_ERROR AC_API acBooleanGetValue(acNode hNode, bool8_t* pValue)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts boolean nodes
 *
 * @param pValue
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - Value of the node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBooleanGetValue </B> retrieves a boolean node's value.
 */
AC_ERROR AC_API acBooleanGetValue(acNode hNode, bool8_t* pValue);

/**
 * @fn AC_ERROR AC_API acBooleanSetValue(acNode hNode, bool8_t value)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts boolean nodes
 *
 * @param value
 *  - Type: bool8_t
 *  - [In] parameter
 *  - Value to set
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acBooleanSetValue </B> sets a boolean node's value.
 */
AC_ERROR AC_API acBooleanSetValue(acNode hNode, bool8_t value);

#ifdef __cplusplus
} // extern "C"
#endif

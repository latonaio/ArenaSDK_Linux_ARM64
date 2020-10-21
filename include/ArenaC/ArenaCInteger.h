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
 * @fn AC_ERROR AC_API acIntegerGetInc(acNode hNode, int64_t* pIncrement)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts integer nodes
 *
 * @param pIncrement
 *  - Type: int64_t*
 *  - [Out] parameter
 *  - Value increment
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acIntegerGetInc </B> retrieves the increment between valid values.
 */
AC_ERROR AC_API acIntegerGetInc(acNode hNode, int64_t* pIncrement);

/**
 * @fn AC_ERROR AC_API acIntegerGetIncMode(acNode hNode, AC_INC_MODE* pIncrementMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts integer nodes
 *
 * @param pIncrementMode
 *  - Type: AC_INC_MODE*
 *  - [Out] parameter
 *  - Increment mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acIntegerGetIncMode </B> retrieves the increment mode.
 */
AC_ERROR AC_API acIntegerGetIncMode(acNode hNode, AC_INC_MODE* pIncrementMode);

/**
 * @fn AC_ERROR AC_API acIntegerGetMax(acNode hNode, int64_t* pMaximum)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts integer nodes
 *
 * @param pMaximum
 *  - Type: int64_t*
 *  - [Out] parameter
 *  - Maximum value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acIntegerGetMax </B> retrieves the maximum value.
 */
AC_ERROR AC_API acIntegerGetMax(acNode hNode, int64_t* pMaximum);

/**
 * @fn AC_ERROR AC_API acIntegerGetMin(acNode hNode, int64_t* pMinimum)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts integer nodes
 *
 * @param pMinimum
 *  - Type: int64_t*
 *  - [Out] parameter
 *  - Minimum value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acIntegerGetMin </B> retrieves the minimum value.
 */
AC_ERROR AC_API acIntegerGetMin(acNode hNode, int64_t* pMinimum);

/**
 * @fn AC_ERROR AC_API acIntegerGetRepresentation(acNode hNode, AC_REPRESENTATION* pRepresentation)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts integer nodes
 *
 * @param pRepresentation
 *  - Type: AC_REPRESENTATION*
 *  - [Out] parameter
 *  - Numeric representation
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acIntegerGetRepresentation </B> retrieves the value representation.
 */
AC_ERROR AC_API acIntegerGetRepresentation(acNode hNode, AC_REPRESENTATION* pRepresentation);

/**
 * @fn AC_ERROR AC_API acIntegerGetUnit(acNode hNode, char* pUnitBuf, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts integer nodes
 *
 * @param pUnitBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Unit of the value
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
 * <B> acIntegerGetValue </B> represents the units of the value.
 */
AC_ERROR AC_API acIntegerGetUnit(acNode hNode, char* pUnitBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acIntegerGetValue(acNode hNode, int64_t* pValue)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts integer nodes
 *
 * @param pValue
 *  - Type: int64_t*
 *  - [Out] parameter
 *  - Value of the node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acIntegerGetValue </B> retrieves the current value.
 */
AC_ERROR AC_API acIntegerGetValue(acNode hNode, int64_t* pValue);

/**
 * @fn AC_ERROR AC_API acIntegerImposeMin(acNode hNode, int64_t imposedMinimum)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts integer nodes
 *
 * @param imposedMinimum
 *  - Type: int64_t
 *  - [In] parameter
 *  - Minimum to impose
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acIntegerImposeMin </B> imposes a minimum. New minimum must be greater
 * than the actual minimum.
 */
AC_ERROR AC_API acIntegerImposeMin(acNode hNode, int64_t imposedMinimum);

/**
 * @fn AC_ERROR AC_API acIntegerImposeMax(acNode hNode, int64_t imposedMaximum)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts integer nodes
 *
 * @param imposedMaximum
 *  - Type: int64_t
 *  - [In] parameter
 *  - Maximum to impose
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acIntegerImposeMax </B> imposes a maximum. New maximum msut be less than
 * the actual maximum.
 */
AC_ERROR AC_API acIntegerImposeMax(acNode hNode, int64_t imposedMaximum);

/**
 * @fn AC_ERROR AC_API acIntegerSetValue(acNode hNode, int64_t value)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts integer nodes
 *
 * @param value
 *  - Type: int64_t
 *  - [In] parameter
 *  - Value to set
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acIntegerSetValue </B> sets a new value.
 */
AC_ERROR AC_API acIntegerSetValue(acNode hNode, int64_t value);

#ifdef __cplusplus
} // extern "C"
#endif

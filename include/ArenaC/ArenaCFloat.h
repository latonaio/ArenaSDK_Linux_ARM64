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
 * @fn AC_ERROR AC_API acFloatSetValue(acNode hNode, double value)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param value
 *  - Type: double
 *  - [In] parameter
 *  - Value to set
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFloatSetValue </B> sets a new value.
 */
AC_ERROR AC_API acFloatSetValue(acNode hNode, double value);

/**
 * @fn AC_ERROR AC_API acFloatGetValue(acNode hNode, double* pValue)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param pValue
 *  - Type: double*
 *  - [Out] parameter
 *  - Current value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFloatGetValue </B> retrieves the current value.
 */
AC_ERROR AC_API acFloatGetValue(acNode hNode, double* pValue);

/**
 * @fn AC_ERROR AC_API acFloatGetMin(acNode hNode, double* pMinimum)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param pMinimum
 *  - Type: double*
 *  - [Out] parameter
 *  - Minimum value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFloatGetMin </B> retrieves the minimum value.
 */
AC_ERROR AC_API acFloatGetMin(acNode hNode, double* pMinimum);

/**
 * @fn AC_ERROR AC_API acFloatGetMax(acNode hNode, double* pMaximum)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param pMaximum
 *  - Type: double*
 *  - [Out] parameter
 *  - Maximum value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFloatGetMax </B> retrieves the maximum value.
 */
AC_ERROR AC_API acFloatGetMax(acNode hNode, double* pMaximum);

/**
 * @fn AC_ERROR AC_API acFloatHasInc(acNode hNode, bool8_t* pHasInc)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param pHasInc
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if the value has a software-imposed increment
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFloatHasInc </B> retrieves whether or not the value has an increment.
 */
AC_ERROR AC_API acFloatHasInc(acNode hNode, bool8_t* pHasInc);

/**
 * @fn AC_ERROR AC_API acFloatGetIncMode(acNode hNode, AC_INC_MODE* pIncMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param pIncMode
 *  - Type: AC_INC_MODE*
 *  - [Out] parameter
 *  - Increment mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFloatGetIncMode </B> retrieves the increment (if there is one).
 */
AC_ERROR AC_API acFloatGetIncMode(acNode hNode, AC_INC_MODE* pIncMode);

/**
 * @fn AC_ERROR AC_API acFloatGetInc(acNode hNode, double* pIncrement)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param pIncrement
 *  - Type: double*
 *  - [Out] parameter
 *  - Increment value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFloatGetInc </B> retrieves the value increment (if there is one). Get
 * the increment after checking whether there is one (acFloatHasInc).
 *
 * @see 
 *  - acFloatHasInc
 */
AC_ERROR AC_API acFloatGetInc(acNode hNode, double* pIncrement);

/**
 * @fn AC_ERROR AC_API acFloatGetRepresentation(acNode hNode, AC_REPRESENTATION* pRepresentation)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
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
 * <B> acFloatGetRepresentation </B> retrieves the value representation.
 */
AC_ERROR AC_API acFloatGetRepresentation(acNode hNode, AC_REPRESENTATION* pRepresentation);

/**
 * @fn AC_ERROR AC_API acFloatGetUnit(acNode hNode, char* pUnitBuf, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param pUnitBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Value unit
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
 * <B> acFloatGetUnit </B> retrieves the units of the value.
 */
AC_ERROR AC_API acFloatGetUnit(acNode hNode, char* pUnitBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acFloatGetDisplayNotation(acNode hNode, AC_DISPLAY_NOTATION* pDisplayNotation)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param pDisplayNotation
 *  - Type: AC_DISPLAY_NOTATION*
 *  - [Out] parameter
 *  - Display notation
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFloatGetDisplayNotation </B> retrieves the best way to display the
 * float, either by floating point, scientific notation, or both depending on the
 * value (automatic).
 */
AC_ERROR AC_API acFloatGetDisplayNotation(acNode hNode, AC_DISPLAY_NOTATION* pDisplayNotation);

/**
 * @fn AC_ERROR AC_API acFloatGetDisplayPrecision(acNode hNode, int64_t* pDisplayPrecision)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param pDisplayPrecision
 *  - Type: int64_t*
 *  - [Out] parameter
 *  - Floating point precision
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFloatGetDisplayPrecision </B> retrieves the floating point precision of
 * the value.
 */
AC_ERROR AC_API acFloatGetDisplayPrecision(acNode hNode, int64_t* pDisplayPrecision);

/**
 * @fn AC_ERROR AC_API acFloatImposeMin(acNode hNode, double imposedMinimum)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param imposedMinimum
 *  - Type: double
 *  - [In] parameter
 *  - Minimum to impose
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFloatImposeMin </B> imposes a minimum. New minimum must be greater than
 * the actual minimum.
 */
AC_ERROR AC_API acFloatImposeMin(acNode hNode, double imposedMinimum);

/**
 * @fn AC_ERROR AC_API acFloatImposeMax(acNode hNode, double imposedMaximum)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts float nodes
 *
 * @param imposedMaximum
 *  - Type: double
 *  - [In] parameter
 *  - Maximum to impose
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acFloatImposeMax </B> imposes a maximum. New maximum must be less than the
 * actual maximum.
 */
AC_ERROR AC_API acFloatImposeMax(acNode hNode, double imposedMaximum);

#ifdef __cplusplus
} // extern "C"
#endif

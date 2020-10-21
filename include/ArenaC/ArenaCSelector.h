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
 * @fn AC_ERROR AC_API acSelectorIsSelector(acNode hNode, bool8_t* pIsSelector)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts selector nodes
 *
 * @param pIsSelector
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if node is a selector
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSelectorIsSelector </B> retrieves whether or not a node is a selector.
 */
AC_ERROR AC_API acSelectorIsSelector(acNode hNode, bool8_t* pIsSelector);

/**
 * @fn AC_ERROR AC_API acSelectorGetNumSelectingFeatures(acNode hNode, size_t* pNumSelectingFeatures)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts selector nodes
 *
 * @param pNumSelectingFeatures
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Number of selecting features
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSelectorIsSelector </B> retrieves the number of selecting features of a
 * selector node.
 */
AC_ERROR AC_API acSelectorGetNumSelectingFeatures(acNode hNode, size_t* pNumSelectingFeatures);

/**
 * @fn AC_ERROR AC_API acSelectorGetSelectingFeature(acNode hNode, size_t index, acNode* phSelectingFeatureNode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts selector nodes
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the selecting feature
 *
 * @param phSelectingFeatureNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Selecting feature (node of type enum entry)
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSelectorIsSelector </B> retrieves a selecting feature by its index.
 */
AC_ERROR AC_API acSelectorGetSelectingFeature(acNode hNode, size_t index, acNode* phSelectingFeatureNode);

/**
 * @fn AC_ERROR AC_API acSelectorGetSelectingFeatureAndAccessMode(acNode hNode, size_t index, acNode* phSelectingFeatureNode, AC_ACCESS_MODE* pAccessMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts selector nodes
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the selecting feature
 *
 * @param phSelectingFeatureNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Selecting feature (node of type enum entry)
 *
 * @param pAccessMode
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - Selecting feature's access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSelectorIsSelector </B> retrieves a selecting feature and its access
 * mode by its index.
 */
AC_ERROR AC_API acSelectorGetSelectingFeatureAndAccessMode(acNode hNode, size_t index, acNode* phSelectingFeatureNode, AC_ACCESS_MODE* pAccessMode);

/**
 * @fn AC_ERROR AC_API acSelectorGetNumSelectedFeatures(acNode hNode, size_t* pNumSelectedFeatures)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts selector node
 *
 * @param pNumSelectedFeatures
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Number of selected features
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSelectorIsSelector </B> retrieves the number of selected features of a
 * selector node.
 */
AC_ERROR AC_API acSelectorGetNumSelectedFeatures(acNode hNode, size_t* pNumSelectedFeatures);

/**
 * @fn AC_ERROR AC_API acSelectorGetSelectedFeature(acNode hNode, size_t index, acNode* phSelectedFeatureNode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts selector nodes
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the selected feature
 *
 * @param phSelectedFeatureNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Selected feature (node of type enum entry)
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSelectorIsSelector </B> retrieves a selected feature by its index.
 */
AC_ERROR AC_API acSelectorGetSelectedFeature(acNode hNode, size_t index, acNode* phSelectedFeatureNode);

/**
 * @fn AC_ERROR AC_API acSelectorGetSelectedFeatureAndAccessMode(acNode hNode, size_t index, acNode* phSelectedFeatureNode, AC_ACCESS_MODE* pAccessMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts selector nodes
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of selected feature
 *
 * @param phSelectedFeatureNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Selected feature (node of type enum entry)
 *
 * @param pAccessMode
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - Selected feature's access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acSelectorIsSelector </B> retrieves a selected feature and its access mode
 * by its index.
 */
AC_ERROR AC_API acSelectorGetSelectedFeatureAndAccessMode(acNode hNode, size_t index, acNode* phSelectedFeatureNode, AC_ACCESS_MODE* pAccessMode);

#ifdef __cplusplus
} // extern "C"
#endif

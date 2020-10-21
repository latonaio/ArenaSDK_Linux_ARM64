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
 * @fn AC_ERROR AC_API acCategoryGetNumFeatures(acNode hNode, size_t* pNumFeatures)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts category nodes
 *
 * @param pNumFeatures
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Number of features
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acCategoryGetNumFeatures </B> retrieves the number of features of a
 * category node.
 */
AC_ERROR AC_API acCategoryGetNumFeatures(acNode hNode, size_t* pNumFeatures);

/**
 * @fn AC_ERROR AC_API acCategoryGetFeature(acNode hNode, size_t index, acNode* hFeatureNode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts category nodes
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the feature
 *
 * @param hFeatureNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Feature node (node of any type)
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acCategoryGetFeatureAndAccessMode </B> retrieves a feature node by its
 * index.
 */
AC_ERROR AC_API acCategoryGetFeature(acNode hNode, size_t index, acNode* hFeatureNode);

/**
 * @fn AC_ERROR AC_API acCategoryGetFeatureAndAccessMode(acNode hNode, size_t index, acNode* hFeatureNode, AC_ACCESS_MODE* pAccessMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - A node
 *  - Accepts category nodes
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the feature node
 *
 * @param hFeatureNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Feature node (node of any type)
 *
 * @param pAccessMode
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - Feature node's access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acCategoryGetFeatureAndAccessMode </B> retrieves a feature node and its
 * access mode by its index.
 */
AC_ERROR AC_API acCategoryGetFeatureAndAccessMode(acNode hNode, size_t index, acNode* hFeatureNode, AC_ACCESS_MODE* pAccessMode);

#ifdef __cplusplus
} // extern "C"
#endif

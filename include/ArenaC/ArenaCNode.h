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
 * @fn AC_ERROR AC_API acNodeGetAccessMode(acNode hNode, AC_ACCESS_MODE* pAccessMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pAccessMode
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - Access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetAccessMode </B> retrieves a node's access mode.
 */
AC_ERROR AC_API acNodeGetAccessMode(acNode hNode, AC_ACCESS_MODE* pAccessMode);

/**
 * @fn AC_ERROR AC_API acNodeGetAlias(acNode hNode, acNode* phAliasNode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param phAliasNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Alias node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetAlias </B> retrieves a node's alias node.
 */
AC_ERROR AC_API acNodeGetAlias(acNode hNode, acNode* phAliasNode);

/**
 * @fn AC_ERROR AC_API acNodeGetCachingMode(acNode hNode, AC_CACHING_MODE* pCachingMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pCachingMode
 *  - Type: AC_CACHING_MODE*
 *  - [Out] parameter
 *  - Caching mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetCachingMode </B> retrieves a node's caching mode.
 */
AC_ERROR AC_API acNodeGetCachingMode(acNode hNode, AC_CACHING_MODE* pCachingMode);

/**
 * @fn AC_ERROR AC_API acNodeGetCastAlias(acNode hNode, acNode* phAliasNode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param phAliasNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Cast alias
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetCastAlias </B> retrieves a node's cast alias.
 */
AC_ERROR AC_API acNodeGetCastAlias(acNode hNode, acNode* phAliasNode);

/**
 * @fn AC_ERROR AC_API acNodeGetNumChildren(acNode hNode, size_t* pNumChildren)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pNumChildren
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Number of child nodes
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetNumChildren </B> retrieves the number of a node's children.
 */
AC_ERROR AC_API acNodeGetNumChildren(acNode hNode, size_t* pNumChildren);

/**
 * @fn AC_ERROR AC_API acNodeGetChild(acNode hNode, size_t index, acNode* phChildNode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the child
 *
 * @param phChildNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Child node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetChild </B> retrieves a node's child node.
 */
AC_ERROR AC_API acNodeGetChild(acNode hNode, size_t index, acNode* phChildNode);

/**
 * @fn AC_ERROR AC_API acNodeGetChildAndAccessMode(acNode hNode, size_t index, acNode* phChildNode, AC_ACCESS_MODE* accessMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the child
 *
 * @param phChildNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Child node
 *
 * @param accessMode
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - Child node's access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetChildAndAccessMode </B> retrieves a node's child node and the
 * child's access mode.
 */
AC_ERROR AC_API acNodeGetChildAndAccessMode(acNode hNode, size_t index, acNode* phChildNode, AC_ACCESS_MODE* accessMode);

/**
 * @fn AC_ERROR AC_API acNodeGetDescription(acNode hNode, char* pDescription, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pDescription
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Description of the node
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
 * <B> acNodeGetDescription </B> retrieves a node's longer description.
 */
AC_ERROR AC_API acNodeGetDescription(acNode hNode, char* pDescription, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeGetDeviceName(acNode hNode, char* pDeviceName, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pDeviceName
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Device name of the node
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
 * <B> acNodeGetDeviceName </B> retrieves a node's device name.
 */
AC_ERROR AC_API acNodeGetDeviceName(acNode hNode, char* pDeviceName, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeGetDisplayName(acNode hNode, char* pDisplayName, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pDisplayName
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Display name
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
 * <B> acNodeGetDisplayName </B> retrieves a node's display name.
 */
AC_ERROR AC_API acNodeGetDisplayName(acNode hNode, char* pDisplayName, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeGetDocuURL(acNode hNode, char* pDocuURL, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pDocuURL
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Document URL
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
 * <B> acNodeGetDocuURL </B> retrieves a node's document URL.
 */
AC_ERROR AC_API acNodeGetDocuURL(acNode hNode, char* pDocuURL, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeGetEventID(acNode hNode, char* pEventIDBuf, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pEventIDBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Event ID
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
 * <B> acNodeGetEventID </B> retrieves a node's event ID.
 */
AC_ERROR AC_API acNodeGetEventID(acNode hNode, char* pEventIDBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeGetName(acNode hNode, char* pNameBuf, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pNameBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Name
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
 * <B> acNodeGetName </B> retrieves a node's name.
 */
AC_ERROR AC_API acNodeGetName(acNode hNode, char* pNameBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeGetFullyQualifiedName(acNode hNode, char* pNameBuf, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - In parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pNameBuf
 *  - Type: char*
 *  - Out parameter
 *  - Accepts null
 *  - Name
 *
 * @param pBufLen
 *  - Type: size_t*
 *  - In/out parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetFullyQualifiedName </B> retrieves a node's fully qualified name.
 */
AC_ERROR AC_API acNodeGetFullyQualifiedName(acNode hNode, char* pNameBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeGetNamespace(acNode hNode, AC_NAMESPACE* pNameSpace)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pNameSpace
 *  - Type: AC_NAMESPACE*
 *  - [Out] parameter
 *  - Namespace
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetNamespace </B> retrieves a node's namespace.
 */
AC_ERROR AC_API acNodeGetNamespace(acNode hNode, AC_NAMESPACE* pNameSpace);

/**
 * @fn AC_ERROR AC_API acNodeGetNumParents(acNode hNode, size_t* pNumParents)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pNumParents
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Number of parent nodes
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetNumParents </B> retrieves the number of a node's parents.
 */
AC_ERROR AC_API acNodeGetNumParents(acNode hNode, size_t* pNumParents);

/**
 * @fn AC_ERROR AC_API acNodeGetParent(acNode hNode, size_t index, acNode* phParentNode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the parent
 *
 * @param phParentNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - The parent node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetParent </B> retrieves a node's parent node.
 */
AC_ERROR AC_API acNodeGetParent(acNode hNode, size_t index, acNode* phParentNode);

/**
 * @fn AC_ERROR AC_API acNodeGetParentAndAccessMode(acNode hNode, size_t index, acNode* phParentNode, AC_ACCESS_MODE* pAccessMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the parent node
 *
 * @param phParentNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - The parent node
 *
 * @param pAccessMode
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - Parent node's access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetParentAndAccessMode </B> retrieves a node's parent node and the
 * parent's access mode.
 */
AC_ERROR AC_API acNodeGetParentAndAccessMode(acNode hNode, size_t index, acNode* phParentNode, AC_ACCESS_MODE* pAccessMode);

/**
 * @fn AC_ERROR AC_API acNodeGetPollingTime(acNode hNode, int64_t* pPollingTime)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pPollingTime
 *  - Type: int64_t*
 *  - [Out] parameter
 *  - Polling time
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetPollingTime </B> retrieves a node's polling time in milliseconds.
 */
AC_ERROR AC_API acNodeGetPollingTime(acNode hNode, int64_t* pPollingTime);

/**
 * @fn AC_ERROR AC_API acNodeGetPrincipalInterfaceType(acNode hNode, AC_INTERFACE_TYPE* pInterfaceType)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pInterfaceType
 *  - Type: AC_INTERFACE_TYPE*
 *  - [Out] parameter
 *  - Node type
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetPrincipalInterfaceType </B> retrieves a node's type.
 */
AC_ERROR AC_API acNodeGetPrincipalInterfaceType(acNode hNode, AC_INTERFACE_TYPE* pInterfaceType);

/**
 * @fn AC_ERROR AC_API acNodeGetProperty(acNode hNode, const char* pPropertyName, char* pPropertyValueBuf, size_t* pPropertyValueBufLen, char* pPropertyAttributeBuf, size_t* pPropertyAttributeBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pPropertyName
 *  - Type: const char*
 *  - [In] parameter
 *  - Accepts null
 *  - List of property names
 *
 * @param pPropertyValueBuf
 *  - Type: char*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @param pPropertyValueBufLen
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Accepts null
 *  - List of property values
 *
 * @param pPropertyAttributeBuf
 *  - Type: char*
 *  - [In/out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value
 *
 * @param pPropertyAttributeBufLen
 *  - Type: size_t*
 *  - [Out] parameter
 *  - The node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetProperty </B> retrieves a node's properties and their values.
 */
AC_ERROR AC_API acNodeGetProperty(acNode hNode, const char* pPropertyName, char* pPropertyValueBuf, size_t* pPropertyValueBufLen, char* pPropertyAttributeBuf, size_t* pPropertyAttributeBufLen);

/**
 * @fn AC_ERROR AC_API acNodeGetNumPropertyNames(acNode hNode, size_t* pNumPropertyNames)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pNumPropertyNames
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Number of node properties
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetNumPropertyNames </B> retrieves the number of a node's property
 * names.
 */
AC_ERROR AC_API acNodeGetNumPropertyNames(acNode hNode, size_t* pNumPropertyNames);

/**
 * @fn AC_ERROR AC_API acNodeGetPropertyName(acNode hNode, size_t index, char* pPropertyNameBuf, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param index
 *  - Type: size_t
 *  - [In] parameter
 *  - Index of the property
 *
 * @param pPropertyNameBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Property name
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
 * <B> acNodeGetPropertyName </B> retrieves a node's property name.
 */
AC_ERROR AC_API acNodeGetPropertyName(acNode hNode, size_t index, char* pPropertyNameBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeGetToolTip(acNode hNode, char* pToolTipBuf, size_t* pBufLen)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pToolTipBuf
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Tool tip
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
 * <B> acNodeGetToolTip </B> retrieves a node's shorter description.
 */
AC_ERROR AC_API acNodeGetToolTip(acNode hNode, char* pToolTipBuf, size_t* pBufLen);

/**
 * @fn AC_ERROR AC_API acNodeInvalidateNode(acNode hNode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeInvalidateNode </B> invalidates a node.
 */
AC_ERROR AC_API acNodeInvalidateNode(acNode hNode);

/**
 * @fn AC_ERROR AC_API acNodeGetVisibility(acNode hNode, AC_VISIBILITY* pVisibility)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pVisibility
 *  - Type: AC_VISIBILITY*
 *  - [Out] parameter
 *  - Visibility
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeGetVisibility </B> retrieves a node's suggested visibility.
 */
AC_ERROR AC_API acNodeGetVisibility(acNode hNode, AC_VISIBILITY* pVisibility);

/**
 * @fn AC_ERROR AC_API acNodeImposeAccessMode(acNode hNode, AC_ACCESS_MODE imposedAccessMode)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param imposedAccessMode
 *  - Type: AC_ACCESS_MODE
 *  - [In] parameter
 *  - Imposed access mode
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeImposeAccessMode </B> imposes an access mode on a node.
 */
AC_ERROR AC_API acNodeImposeAccessMode(acNode hNode, AC_ACCESS_MODE imposedAccessMode);

/**
 * @fn AC_ERROR AC_API acNodeImposeVisibility(acNode hNode, AC_VISIBILITY imposedVisibility)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param imposedVisibility
 *  - Type: AC_VISIBILITY
 *  - [In] parameter
 *  - Imposed visibility
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeImposeVisibility </B> imposes a visibility setting on a node.
 */
AC_ERROR AC_API acNodeImposeVisibility(acNode hNode, AC_VISIBILITY imposedVisibility);

/**
 * @fn AC_ERROR AC_API acNodeIsCachable(acNode hNode, bool8_t* pIsCachable)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pIsCachable
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if node value is cachable
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeIsCachable </B> retrieves whether or not a node is cachable.
 */
AC_ERROR AC_API acNodeIsCachable(acNode hNode, bool8_t* pIsCachable);

/**
 * @fn AC_ERROR AC_API acNodeIsDeprecated(acNode hNode, bool8_t* pIsDeprecated)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pIsDeprecated
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if node is deprecated
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeIsDeprecated </B> retrieves whether or not a node is deprecated.
 */
AC_ERROR AC_API acNodeIsDeprecated(acNode hNode, bool8_t* pIsDeprecated);

/**
 * @fn AC_ERROR AC_API acNodeIsFeature(acNode hNode, bool8_t* pIsFeature)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pIsFeature
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if node is a feature
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeIsFeature </B> retrieves whether or not a node is a feature.
 */
AC_ERROR AC_API acNodeIsFeature(acNode hNode, bool8_t* pIsFeature);

/**
 * @fn AC_ERROR AC_API acNodeIsStreamable(acNode hNode, bool8_t* pIsStreamable)
 *
 * @param hNode
 *  - Type: acNode
 *  - [In] parameter
 *  - The node
 *  - Accepts any node type
 *
 * @param pIsStreamable
 *  - Type: bool8_t*
 *  - [Out] parameter
 *  - True if node is streamable
 *  - Otherwise, false
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acNodeIsStreamable </B> retrieves whether or not a node is streamable.
 */
AC_ERROR AC_API acNodeIsStreamable(acNode hNode, bool8_t* pIsStreamable);

#ifdef __cplusplus
} // extern "C"
#endif

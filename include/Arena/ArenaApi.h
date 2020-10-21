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

#if (defined _WIN32 || defined _WIN64)
#ifdef ARENA_EXPORTS
#define ARENA_API __declspec(dllexport)
#else
#define ARENA_API __declspec(dllimport)
#endif
#else
#define ARENA_API
#endif

#if defined __linux__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
#include <GenICam.h>
#include <GenApi/Types.h>
#include <GenApi/EnumClasses.h>

#if defined __linux__
#pragma GCC diagnostic pop
#endif

#include "Arena.h"
#include "ArenaDefs.h"
#include "DeviceInfo.h"
#include "FeatureStream.h"
#include "GenApiCustom.h"
#include "IBuffer.h"
#include "IChunkData.h"
#include "IDevice.h"
#include "IImage.h"
#include "ImageFactory.h"
#include "ISystem.h"
#include "PFNC.h"
#include "PFNCCustom.h"

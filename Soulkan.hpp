/*Include guards to avoid including this .hpp file multiple times*/
#ifndef SOULKAN_HPP
#define SOULKAN_HPP
#pragma once

/*Defining namespace as a macro so that the user can change it*/
#define SOULKAN_NAMESPACE sk


/*Includes from the std library or from needed tools*/
#include <cstdarg>
#include <chrono>
#include <fstream>
#include <deque>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#define GLFW_INCLUDE_VULKAN


namespace SOULKAN_NAMESPACE
{
	/*Types :
	*Result types
	*Info   type
	*Error  types
	*/

	/*@brief The main return value of soulkan functions, containing :
	*@param value, the variable containing the actual return value
	*@param error, the variable containing the error message
	*/
	template<class V, class E>
	struct SkResult
	{
		V value;
		E error;

		SkResult(V val, E err)
			: value(std::move(val)), error(std::move(err))
		{
		}

	};

	/*@brief A very barebone pair struct used for bundling up return values :
	* @param a the first value of the pair
	* @param b the second value of the pair
	*/
	template<class T1, class T2>
	struct Pair
	{
		T1 a;
		T2 b;

		Pair(T1 first, T2 second)
			: a(first), b(second)
		{

		}

	};

	/*@brief The main return value of soulkan test functions, containing :
	* @param totalFrames The total number of frames elapsed during the test
	* @param totalTimeElapsed The total time elapsed (in seconds) during the test
	* @param averageFramesPerSecond The average FPS during the test
	* @param averageFrametime The average frametime (in miliseconds) during the test
	*/
	struct SkTestData
	{
		double totalFrames;
		double totalTimeElapsed;
		double initTimeElapsed;

		double averageFramesPerSecond;
		double averageFrametime;

		SkTestData(double totalF, double totalTime, double initTime, double averageFps, double averageFt)
			: totalFrames(totalF), totalTimeElapsed(totalTime), initTimeElapsed(initTime), averageFramesPerSecond(averageFps), averageFrametime(averageFt)
		{
		}

		SkTestData()
		{
			totalFrames = 0;
			totalTimeElapsed = 0;
			initTimeElapsed = 0;

			averageFramesPerSecond = 0;
			averageFrametime = 0;
		}
	};

	/*@brief Main function of a particular queue family (simplified)
	*@param GENERAL   : Can do everything the other queues can
	*@param GRAPHICS  : The specialized queue to use for graphic operations
	*@param PRESENT   : The specialized queue to use for present operations
	*@param COMPUTE   : The specialized queue to use for compute operations
	*@param TRANSFER  : The specialized queue to use for transfer operations
	*@param DEBUG_TMP : Reserved for future use
	*/
	enum class QueueFamilyType
	{
		GENERAL = 0,
		GRAPHICS = 1,
		PRESENT = 2,
		COMPUTE = 3,
		TRANSFER = 4,
		DEBUG_TMP = 5
	};

	/*@brief Enum containing associations of MemorySize = Equivalent power of 2*/
	enum class MemorySize : uint64_t
	{
		ZERO = 0,

		B_8 = 8,            // 8    Bytes
		B_16 = 16,           // 16   Bytes
		B_32 = 32,           // 32   Bytes 
		B_64 = 64,           // 64   Bytes
		B_128 = 128,          // 128  Bytes
		B_256 = 256,          // 256  Bytes
		B_512 = 512,          // 512  Bytes

		KB_1 = 1024,        // 1    Kilobytes = 1 024         bytes
		KB_2 = 2048,        // 2    Kilobytes = 2 048         bytes
		KB_4 = 4096,        // 4    Kilobytes = 4 096         bytes
		KB_8 = 8192,        // 8    Kilobytes = 8 192         bytes
		KB_16 = 16384,       // 16   Kilobytes = 16 384        bytes
		KB_32 = 32768,       // 32   Kilobytes = 32 768        bytes
		KB_64 = 65536,       // 64   Kilobytes = 65 536        bytes
		KB_128 = 131072,      // 128  Kilobytes = 131 072       bytes
		KB_256 = 262144,      // 256  Kilobytes = 262 144       bytes
		KB_512 = 524288,      // 512  Kilobytes = 524 288       bytes

		MB_1 = 1048576,     // 1    Megabytes = 1 048 576     bytes
		MB_2 = 2097152,     // 2    Megabytes = 2 097 152     bytes
		MB_4 = 4194304,     // 4    Megabytes = 4 194 304     bytes
		MB_8 = 8388608,     // 8    Megabytes = 8 388 608     bytes
		MB_16 = 16777216,    // 16   Megabytes = 16 777 216    bytes
		MB_32 = 33554432,    // 32   Megabytes = 33 554 432    bytes
		MB_64 = 67108864,    // 64   Megabytes = 67 108 864    bytes
		MB_128 = 134217728,   // 128  Megabytes = 134 217 728   bytes
		MB_256 = 268435456,   // 256  Megabytes = 268 435 456   bytes
		MB_512 = 536870912,   // 512  Megabytes = 536 870 912   bytes

		MB_1024 = 1073741824, // 1024 Megabytes = 1 073 741 824 bytes
		MB_2048 = 2147483648, // 2048 Megabytes = 2 147 483 648 bytes
		MB_4096 = 4294967296, // 4096 Megabytes = 4 294 967 296 bytes

		GB_1 = MB_1024,       // 1 Gigabyte = 1024 Megabytes
		GB_2 = MB_2048,       // 2 Gigabyte = 2048 Megabytes
		GB_4 = MB_4096        // 4 Gigabyte = 4096 Megabytes
	};

	//Not using it directly right now, needs updating
	enum class MemoryBlockSize : uint64_t
	{
		ZERO = static_cast<uint64_t>(MemorySize::ZERO),
		MINIMUM = static_cast<uint64_t>(MemorySize::MB_2),
		TINY = static_cast<uint64_t>(MemorySize::MB_4),
		LITTLE = static_cast<uint64_t>(MemorySize::MB_8),
		SMALL = static_cast<uint64_t>(MemorySize::MB_16),
		MEDIUM = static_cast<uint64_t>(MemorySize::MB_32),
		LARGE = static_cast<uint64_t>(MemorySize::MB_64),
		MASSIVE = static_cast<uint64_t>(MemorySize::MB_128),
		MAXIMUM = static_cast<uint64_t>(MemorySize::MB_256)

	};

	//Not using it directly right now, needs updating
	enum class MemoryPoolSize : uint64_t
	{
		ZERO = static_cast<uint64_t>(MemorySize::ZERO),
		MINIMUM = static_cast<uint64_t>(MemorySize::MB_128),
		TINY = static_cast<uint64_t>(MemorySize::MB_256),
		SMALL = static_cast<uint64_t>(MemorySize::MB_512),
		MEDIUM = static_cast<uint64_t>(MemorySize::MB_1024),
		LARGE = static_cast<uint64_t>(MemorySize::MB_2048),
		MAXIMUM = static_cast<uint64_t>(MemorySize::MB_4096)
	};

	/*@brief Placeholder enum when an appropriate enum has not been written yet*/
	enum class UndefinedError
	{
		NO_ERROR = 0,
		UNDEFINED_ERROR = 1
	};

	/*@brief Enum containing error messages concerning Tests*/
	enum class TestError
	{
		NO_ERROR = 0
	};

	/*@brief Enum containing error messages concerning GLFW*/
	enum class GLFWError
	{
		NO_ERROR = 0,
		INIT_ERROR = 1,
		WINDOW_CREATION_ERROR = 2,
		TERMINATION_ERROR = 3
	};

	/*@brief Enum containing error messages concerning the debug utils*/
	enum class DebugUtilsError
	{
		NO_ERROR = 0,
		DEBUG_CALLBACK_CI_ERROR = 1,
		DEBUG_CALLBACK_CREATION_ERROR = 2
	};

	/*@brief Enum containing error messages concerning the instance*/
	enum class InstanceError
	{
		NO_ERROR = 0,
		GET_REQUIRED_INSTANCE_EXTENSIONS_ERROR = 1,
		APPLICATION_INFO_CREATION_ERROR = 2,
		INSTANCE_CREATION_ERROR = 3,
		SURFACE_CREATION_ERROR = 4
	};

	/*@brief Enum containing error messages concerning the physical device*/
	enum class PhysicalDeviceError
	{
		NO_ERROR = 0,
		UNDEFINED_ERROR = 1,
		NO_PHYSICAL_DEVICE_FOUND = 2,
		NO_SUITABLE_PHYSICAL_DEVICE_FOUND = 3,
		NO_SUITABLE_QUEUES_FOUND = 4,
		NO_PRESENT_MODES = 5,
		NO_SURFACE_FORMATS = 6,
		UNDEFINED_FORMAT = 7,
		GLFW_FRAMEBUFFER_SIZE_ERROR = 8
	};

	/*@brief Enum containing error messages concerning the logical device (device)*/
	enum class DeviceError
	{
		NO_ERROR = 0,
		WRONG_QUEUE_FAMILY_INDEX = 1,
		DEVICE_EXTENSIONS_EMPTY = 2,
		DEVICE_CREATION_ERROR = 3,
		DEVICE_DESTRUCTION_ERROR = 4,
		QUEUE_CREATION_ERROR = 5,
		QUEUE_SUBMIT_ERROR = 6,
		QUEUE_PRESENT_ERROR = 7
	};

	/*@brief Enum containing error messages concerning device queues*/
	enum class QueueError
	{
		NO_ERROR = 0,
		EMPTY_CONCENTRATED_QUEUE_VECTOR = 1
	};

	/*@brief Enum containing error messages concerning the swapchain*/
	enum class SwapchainError
	{
		NO_ERROR = 0,
		NO_FORMAT_FOUND = 1,
		SWAPCHAIN_CREATION_ERROR = 2,
		SWAPCHAIN_DESTRUCTION_ERROR = 3,
		GET_SWAPCHAIN_IMAGES_ERROR = 4,
		SWAPCHAIN_IMAGE_VIEW_CREATION_ERROR = 5,
		SWAPCHAIN_IMAGE_VIEW_DESTRUCTION_ERROR = 6
	};

	/*@brief Enum containing error messages concerning the command pool*/
	enum class CommandPoolError
	{
		NO_ERROR = 0,
		WRONG_QUEUE_INDEX = 1,
		COMMAND_POOL_CREATION_ERROR = 2,
		COMMAND_POOL_DESTRUCTION_ERROR = 3
	};

	/*@brief Enum containing error messages concerning the command buffer*/
	enum class CommandBufferError
	{
		NO_ERROR = 0,
		COMMAND_BUFFER_ALLOCATION_ERROR = 1,
		COMMAND_BUFFER_BEGIN_ERROR = 2,
		COMMAND_BUFFER_END_ERROR = 3
	};

	/*@brief Enum containing error messages concerning the render pass*/
	enum class RenderPassError
	{
		NO_ERROR = 0,
		RENDER_PASS_CREATION_ERROR = 1,
		RENDER_PASS_DESTRUCTION_ERROR = 2,
		RENDER_PASS_BEGIN_ERROR = 3,
		RENDER_PASS_END_ERROR = 4
	};

	/*@brief Enum containing error messages concerning the framebuffer*/
	enum class FramebufferError
	{
		NO_ERROR = 0,
		FRAMEBUFFER_CREATION_ERROR = 1,
		FRAMEBUFFER_DESTRUCTION_ERROR = 2
	};

	/*@brief Enum containing error messages concerning the renderpass*/
	enum class SyncError
	{
		NO_ERROR = 0,
		FENCE_CREATION_ERROR = 1,
		SEMAPHORE_CREATION_ERROR = 2,
		FENCE_DESTRUCTION_ERROR = 3,
		SEMAPHORE_DESTRUCTION_ERROR = 4
	};

	/*@brief Enum containing error messages concerning the drawing process*/
	enum class DrawingError
	{
		NO_ERROR = 0
	};

	/*@brief Enum containing error messages concerning the shaders*/
	enum class ShaderError
	{
		NO_ERROR = 0,
		SHADER_FILE_OPENING_ERROR = 1,
		SHADER_MODULE_CREATION_ERROR = 2,
		SHADER_MODULE_DELETION_ERROR = 3
	};

	/*@brief Enum containing error messages concerning the graphics pipeline*/
	enum class GraphicsPipelineError
	{
		NO_ERROR = 0,
		PIPELINE_SHADER_STAGE_CREATE_INFO_ERROR = 1,
		PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO_ERROR = 2,
		PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO_ERROR = 3,
		PIPELINE_RASTERIZATION_STATE_CREATE_INFO_ERROR = 4,
		PIPELINE_MULTISAMPLE_STATE_CREATE_INFO_ERROR = 5,
		PIPELINE_COLOR_BLEND_ATTACHMENT_STATE_ERROR = 6,
		VIEWPORT_CREATION_ERROR = 7,
		SCISSOR_CREATION_ERROR = 8,
		PIPELINE_VIEWPORT_STATE_CREATE_INFO_ERROR = 9,
		PIPELINE_COLOR_BLEND_STATE_CREATE_INFO_ERROR = 10,
		PIPELINE_LAYOUT_CREATE_INFO_ERROR = 11,
		PIPELINE_LAYOUT_CREATION_ERROR = 12,
		PIPELINE_LAYOUT_DESTRUCTION_ERROR = 13,
		GRAPHICS_PIPELINE_CREATE_INFO_ERROR = 14,
		PIPELINE_CREATION_ERROR = 15,
		PIPELINE_DELETION_ERROR = 16
	};

	/*@brief Enum containing error messages concerning the memory allocator and its allocations*/
	enum class AllocationError
	{
		NO_ERROR = 0,
		UNDEFINED_ERROR = 1,
		COULD_NOT_GET_MEMORY_TYPE_INDEX_ERROR = 2,
		MEMORY_ALLOCATION_ERROR = 3,
		INSUFFICIENT_FREE_MEMORY_ERROR = 4,
		BIND_BUFFER_MEMORY_ERROR = 5,
		MAP_MEMORY_ERROR = 6
	};

	/*@brief Enum containing error messages concerning the buffers*/
	enum class BufferError
	{
		NO_ERROR = 0,
		BUFFER_CREATION_ERROR = 1,
		BUFFER_DESTRUCTION_ERROR = 2
	};

	/*@brief Enum containing error messages concerning meshes*/
	enum class MeshError
	{
		NO_ERROR = 0,
		TRIANGLE_MESH_LOADING_ERROR = 1
	};

	/*@brief Enum containing error messages concerning the maths*/
	enum class MathError
	{
		NO_ERROR = 0,
		DIVIDING_BY_ZERO_ERROR = 1
	};

	/*to_string() implementation for every enum class, inspired by vulkan.hpp*/
	inline std::string to_string(QueueFamilyType value)
	{
		switch (value)
		{
		case QueueFamilyType::GENERAL: return "GENERAL";
		case QueueFamilyType::GRAPHICS: return "GRAPHICS";
		case QueueFamilyType::PRESENT: return "PRESENT";
		case QueueFamilyType::COMPUTE: return "COMPUTE";
		case QueueFamilyType::TRANSFER: return "TRANSFER";
		case QueueFamilyType::DEBUG_TMP: return "DEBUG_TMP";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(UndefinedError value)
	{
		switch (value)
		{
		case UndefinedError::NO_ERROR:        return "NO_ERROR";
		case UndefinedError::UNDEFINED_ERROR: return "UNDEFINED_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(TestError value)
	{
		switch (value)
		{
		case TestError::NO_ERROR: return "NO_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(GLFWError value)
	{
		switch (value)
		{
		case GLFWError::NO_ERROR: return "NO_ERROR";
		case GLFWError::INIT_ERROR: return "INIT_ERROR";
		case GLFWError::WINDOW_CREATION_ERROR: return "WINDOW_CREATION_ERROR";
		case GLFWError::TERMINATION_ERROR: return "TERMINATION_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(DebugUtilsError value)
	{
		switch (value)
		{
		case DebugUtilsError::NO_ERROR: return "NO_ERROR";
		case DebugUtilsError::DEBUG_CALLBACK_CI_ERROR: return "DEBUG_CALLBACK_CI_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(InstanceError value)
	{
		switch (value)
		{
		case InstanceError::NO_ERROR: return "NO_ERROR";
		case InstanceError::GET_REQUIRED_INSTANCE_EXTENSIONS_ERROR: return "GET_REQUIRED_INSTANCE_EXTENSIONS_ERROR";
		case InstanceError::APPLICATION_INFO_CREATION_ERROR: return "APPLICATION_INFO_CREATION_ERROR";
		case InstanceError::INSTANCE_CREATION_ERROR: return "INSTANCE_CREATION_ERROR";
		case InstanceError::SURFACE_CREATION_ERROR: return "SURFACE_CREATION_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(PhysicalDeviceError value)
	{
		switch (value)
		{
		case PhysicalDeviceError::NO_ERROR:                          return "NO_ERROR";
		case PhysicalDeviceError::NO_PHYSICAL_DEVICE_FOUND:          return "NO_PHYSICAL_DEVICE_FOUND";
		case PhysicalDeviceError::NO_SUITABLE_PHYSICAL_DEVICE_FOUND: return "NO_SUITABLE_PHYSICAL_DEVICE_FOUND";
		case PhysicalDeviceError::NO_SUITABLE_QUEUES_FOUND:          return "NO_PRESENT_MODES";
		case PhysicalDeviceError::NO_PRESENT_MODES:                  return "NO_PRESENT_MODES";
		case PhysicalDeviceError::NO_SURFACE_FORMATS:                return "NO_SURFACE_FORMATS";
		case PhysicalDeviceError::UNDEFINED_FORMAT:                  return "UNDEFINED_FORMAT";
		case PhysicalDeviceError::GLFW_FRAMEBUFFER_SIZE_ERROR:       return "GLFW_FRAMEBUFFER_SIZE_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(DeviceError value)
	{
		switch (value)
		{
		case DeviceError::NO_ERROR: return "NO_ERROR";
		case DeviceError::WRONG_QUEUE_FAMILY_INDEX: return "WRONG_QUEUE_FAMILY_INDEX";
		case DeviceError::DEVICE_EXTENSIONS_EMPTY: return "DEVICE_EXTENSIONS_EMPTY";
		case DeviceError::DEVICE_CREATION_ERROR: return "DEVICE_CREATION_ERROR";
		case DeviceError::DEVICE_DESTRUCTION_ERROR: return "DEVICE_DESTRUCTION_ERROR";
		case DeviceError::QUEUE_CREATION_ERROR: return "QUEUE_CREATION_ERROR";
		case DeviceError::QUEUE_SUBMIT_ERROR: return "QUEUE_SUBMIT_ERROR";
		case DeviceError::QUEUE_PRESENT_ERROR: return "QUEUE_PRESENT_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(QueueError value)
	{
		switch (value)
		{
		case QueueError::NO_ERROR: return "NO_ERROR";
		case QueueError::EMPTY_CONCENTRATED_QUEUE_VECTOR: return "EMPTY_CONCENTRATED_QUEUE_VECTOR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(SwapchainError value)
	{
		switch (value)
		{
		case SwapchainError::NO_ERROR: return "NO_ERROR";
		case SwapchainError::NO_FORMAT_FOUND: return "NO_FORMAT_FOUND";
		case SwapchainError::SWAPCHAIN_CREATION_ERROR: return "SWAPCHAIN_CREATION_ERROR";
		case SwapchainError::SWAPCHAIN_DESTRUCTION_ERROR: return "SWAPCHAIN_DESTRUCTION_ERROR";
		case SwapchainError::GET_SWAPCHAIN_IMAGES_ERROR: return "GET_SWAPCHAIN_IMAGES_ERROR";
		case SwapchainError::SWAPCHAIN_IMAGE_VIEW_CREATION_ERROR: return "SWAPCHAIN_IMAGE_VIEW_CREATION_ERROR";
		case SwapchainError::SWAPCHAIN_IMAGE_VIEW_DESTRUCTION_ERROR: return "SWAPCHAIN_IMAGE_VIEW_DESTRUCTION_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(CommandPoolError value)
	{
		switch (value)
		{
		case CommandPoolError::NO_ERROR: return "NO_ERROR";
		case CommandPoolError::WRONG_QUEUE_INDEX: return "WRONG_QUEUE_INDEX";
		case CommandPoolError::COMMAND_POOL_CREATION_ERROR: return "COMMAND_POOL_CREATION_ERROR";
		case CommandPoolError::COMMAND_POOL_DESTRUCTION_ERROR: return "COMMAND_POOL_DESTRUCTION_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(CommandBufferError value)
	{
		switch (value)
		{
		case CommandBufferError::NO_ERROR: return "NO_ERROR";
		case CommandBufferError::COMMAND_BUFFER_ALLOCATION_ERROR: return "COMMAND_BUFFER_ALLOCATION_ERROR";
		case CommandBufferError::COMMAND_BUFFER_BEGIN_ERROR: return "COMMAND_BUFFER_BEGIN_ERROR";
		case CommandBufferError::COMMAND_BUFFER_END_ERROR: return "COMMAND_BUFFER_END_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(RenderPassError value)
	{
		switch (value)
		{
		case RenderPassError::NO_ERROR: return "NO_ERROR";
		case RenderPassError::RENDER_PASS_CREATION_ERROR: return "RENDER_PASS_CREATION_ERROR";
		case RenderPassError::RENDER_PASS_DESTRUCTION_ERROR: return "RENDER_PASS_DESTRUCTION_ERROR";
		case RenderPassError::RENDER_PASS_BEGIN_ERROR: return "RENDER_PASS_BEGIN_ERROR";
		case RenderPassError::RENDER_PASS_END_ERROR: return "RENDER_PASS_END_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(FramebufferError value)
	{
		switch (value)
		{
		case FramebufferError::NO_ERROR: return "NO_ERROR";
		case FramebufferError::FRAMEBUFFER_CREATION_ERROR: return "FRAMEBUFFER_CREATION_ERROR";
		case FramebufferError::FRAMEBUFFER_DESTRUCTION_ERROR: return "FRAMEBUFFER_DESTRUCTION_ERROR";
		default: return "Invalid enum value";
		}

	}

	inline std::string to_string(SyncError value)
	{
		switch (value)
		{
		case SyncError::NO_ERROR: return "NO_ERROR";
		case SyncError::FENCE_CREATION_ERROR: return "FENCE_CREATION_ERROR";
		case SyncError::SEMAPHORE_CREATION_ERROR: return "SEMAPHORE_CREATION_ERROR";
		case SyncError::FENCE_DESTRUCTION_ERROR: return "FENCE_DESTRUCTION_ERROR";
		case SyncError::SEMAPHORE_DESTRUCTION_ERROR: return "SEMAPHORE_DESTRUCTION_ERROR";
		default: return "Invalid enum value";
		}

	}

	inline std::string to_string(DrawingError value)
	{
		switch (value)
		{
		case DrawingError::NO_ERROR: return "NO_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(ShaderError value)
	{
		switch (value)
		{
		case ShaderError::NO_ERROR: return "NO_ERROR";
		case ShaderError::SHADER_MODULE_CREATION_ERROR: return "SHADER_MODULE_CREATION_ERROR";
		case ShaderError::SHADER_MODULE_DELETION_ERROR: return "SHADER_MODULE_DELETION_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(MathError value)
	{
		switch (value)
		{
		case MathError::NO_ERROR: return "NO_ERROR";
		case MathError::DIVIDING_BY_ZERO_ERROR: return "DIVIDING_BY_ZERO_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(GraphicsPipelineError value)
	{
		switch (value)
		{
		case GraphicsPipelineError::NO_ERROR: return "NO_ERROR";
		case GraphicsPipelineError::PIPELINE_SHADER_STAGE_CREATE_INFO_ERROR: return "PIPELINE_SHADER_STAGE_CREATE_INFO_ERROR";
		case GraphicsPipelineError::PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO_ERROR: return "PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO_ERROR";
		case GraphicsPipelineError::PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO_ERROR: return "PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO_ERROR";
		case GraphicsPipelineError::PIPELINE_RASTERIZATION_STATE_CREATE_INFO_ERROR: return "PIPELINE_RASTERIZATION_STATE_CREATE_INFO_ERROR";
		case GraphicsPipelineError::PIPELINE_MULTISAMPLE_STATE_CREATE_INFO_ERROR: return "PIPELINE_MULTISAMPLE_STATE_CREATE_INFO_ERROR";
		case GraphicsPipelineError::PIPELINE_COLOR_BLEND_ATTACHMENT_STATE_ERROR: return "PIPELINE_COLOR_BLEND_ATTACHMENT_STATE_ERROR";
		case GraphicsPipelineError::VIEWPORT_CREATION_ERROR: return "VIEWPORT_CREATION_ERROR";
		case GraphicsPipelineError::SCISSOR_CREATION_ERROR: return "SCISSOR_CREATION_ERROR";
		case GraphicsPipelineError::PIPELINE_VIEWPORT_STATE_CREATE_INFO_ERROR: return "PIPELINE_VIEWPORT_STATE_CREATE_INFO_ERROR";
		case GraphicsPipelineError::PIPELINE_COLOR_BLEND_STATE_CREATE_INFO_ERROR: return "PIPELINE_COLOR_BLEND_STATE_CREATE_INFO_ERROR";
		case GraphicsPipelineError::PIPELINE_LAYOUT_CREATE_INFO_ERROR: return "PIPELINE_LAYOUT_CREATE_INFO_ERROR";
		case GraphicsPipelineError::PIPELINE_LAYOUT_CREATION_ERROR: return "PIPELINE_LAYOUT_CREATION_ERROR";
		case GraphicsPipelineError::PIPELINE_LAYOUT_DESTRUCTION_ERROR: return "PIPELINE_LAYOUT_DESTRUCTION_ERROR";
		case GraphicsPipelineError::GRAPHICS_PIPELINE_CREATE_INFO_ERROR: return "GRAPHICS_PIPELINE_CREATE_INFO_ERROR";
		case GraphicsPipelineError::PIPELINE_CREATION_ERROR: return "PIPELINE_CREATION_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(AllocationError value)
	{
		switch (value)
		{
		case AllocationError::NO_ERROR:                              return "NO_ERROR";
		case AllocationError::UNDEFINED_ERROR:                       return "UNDEFINED_ERROR";
		case AllocationError::COULD_NOT_GET_MEMORY_TYPE_INDEX_ERROR: return "COULD_NOT_GET_MEMORY_TYPE_INDEX_ERROR";
		case AllocationError::MEMORY_ALLOCATION_ERROR:               return "MEMORY_ALLOCATION_ERROR";
		case AllocationError::INSUFFICIENT_FREE_MEMORY_ERROR:        return "INSUFFICIENT_FREE_MEMORY_ERROR";
		case AllocationError::BIND_BUFFER_MEMORY_ERROR:              return "BIND_BUFFER_MEMORY_ERROR";
		case AllocationError::MAP_MEMORY_ERROR:                      return "MAP_MEMORY_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(BufferError value)
	{
		switch (value)
		{
		case BufferError::NO_ERROR:                 return "NO_ERROR";
		case BufferError::BUFFER_CREATION_ERROR:    return "BUFFER_CREATION_ERROR";
		case BufferError::BUFFER_DESTRUCTION_ERROR: return "BUFFER_DESTRUCTION_ERROR";
		default: return "Invalid enum value";
		}
	}

	inline std::string to_string(MeshError value)
	{
		switch (value)
		{
		case MeshError::NO_ERROR:                    return "NO_ERROR";
		case MeshError::TRIANGLE_MESH_LOADING_ERROR: return "TRIANGLE_MESH_LOADING_ERROR";
		default: return "Invalid enum value";
		}
	}

	/*Debug / Util functions*/

	/*@brief Checks if a given SkResult contains an error
	*@param result SkResult, containing value and error, returned from a function
	*@return Boolean indicating if result contains an error
	*/
	template<class V, class E>
	constexpr inline bool error(SkResult<V, E> result) noexcept
	{
		if (result.error != E::NO_ERROR)
		{
			return true;
		}

		else
		{
			return false;
		}
	}

	/*@brief Returns a formatted error message containing error type and result value and error types
	*@param result  SkResult, containing value and error, returned from a function
	*@return Formatted error message as std::string
	*/
	template<class V, class E>
	constexpr inline std::string returnError(SkResult<V, E> result) noexcept
	{
		return std::string("ERROR : " + to_string(static_cast<E>(result.error)) + " when returning SkResult(" + typeid(V).name() + ", " + typeid(E).name() + ")\n");
	}

	/*@brief Checks if a given SkResult contains an error using error() and then logs it
	*@param result  SkResult, containing value and error, returned from a function
	*@return Boolean indicating if result contains an error
	*/
	template<class V, class E>
	constexpr inline bool logError(SkResult<V, E> result) noexcept
	{
		if (error(result))
		{
			std::cout << returnError(result) << std::endl;
			return 1;
		}

		return 0;
	}

	/*@brief Calls logError() on result and then returns the value of the result
	*@param result  SkResult, containing value and error, returned from a function
	*@return The value of result
	*/
	template<class V, class E>
	inline V retLog(SkResult<V, E> result) noexcept
	{
		logError(result);

		return result.value;
	}

	/*@brief Checks if result contains an error using error() and then returns it if it does, else return the current error
	*@param result SkResult, containing value and error, returned from a function
	*@param err the current error to be compared against result.error
	*@return The error of result if result contains an error, else returns err
	*/
	template<class V, class E>
	inline E affectError(SkResult<V, E> result, E err) noexcept
	{
		if (error(result))
		{
			return result.error;
		}
		else
		{
			return err;
		}
	}

	/*@brief A barebone deque containing deletion/cleanup functions*/
	struct DeletionQueue
	{
		std::deque<std::function<void()>> deletors;

		/*@brief Takes an rvalue reference to a function and pushes it back in the deque
		* @param function the rvalue reference to be pushed back inside the deque
		*/
		void push_func(std::function<void()>&& function)
		{
			deletors.emplace_back(function);
		}

		/*@brief Executes the pushed back functions in the reverse order they were pushed back in the deque*/
		void flush()
		{
			for (auto it = deletors.rbegin(); it != deletors.rend(); it++)
			{
				(*it)();
			}

			deletors.clear();
		}
	};

	/*****************************************************************Maths structs and functions******************************************************************************/

	/*Structs first*/

	/*Typical Vec2 holding x and y values*/
	struct Vec2
	{
		Vec2(float posX, float posY)
		{
			x = posX;
			y = posY;
		}

		Vec2()
		{
			x = 0.0f;
			y = 0.0f;
		}

		/*Operators*/
		inline friend bool operator==(const Vec2 lhs, const Vec2 rhs)
		{
			return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
		}

		inline friend bool operator!=(const Vec2 lhs, const Vec2 rhs)
		{
			return !(lhs == rhs);
		}

		inline friend Vec2 operator+(const Vec2 lhs, const Vec2 rhs)
		{
			return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
		}

		inline friend Vec2 operator+=(const Vec2 lhs, const Vec2 rhs)
		{
			return lhs + rhs;
		}

		inline friend Vec2 operator-(const Vec2 lhs, const Vec2 rhs)
		{
			return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
		}

		inline friend Vec2 operator-=(const Vec2 lhs, const Vec2 rhs)
		{
			return lhs - rhs;
		}

		float x;
		float y;
	};

	/*Typical Vec3 holding x, y and z values, constructor accepts Vec2 if needed*/
	struct Vec3
	{
		Vec3(float posX, float posY, float posZ)
			: x(posX), y(posY), z(posZ)
		{
		}

		Vec3(Vec2 vec2, float posZ)
			: x(vec2.x), y(vec2.y), z(posZ)
		{
		}

		Vec3()
			: x(0.0f), y(0.0f), z(0.0f)
		{
		}

		inline friend bool operator==(const Vec3 lhs, const Vec3 rhs)
		{
			return ((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z));
		}

		inline friend bool operator!=(const Vec3 lhs, const Vec3 rhs)
		{
			return !(lhs == rhs);
		}

		inline friend Vec3 operator+(const Vec3 lhs, const Vec3 rhs)
		{
			return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
		}

		inline friend Vec3 operator+=(const Vec3 lhs, const Vec3 rhs)
		{
			return lhs + rhs;
		}

		inline friend Vec3 operator-(const Vec3 lhs, const Vec3 rhs)
		{
			return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
		}

		inline friend Vec3 operator-=(const Vec3 lhs, const Vec3 rhs)
		{
			return lhs - rhs;
		}

		float x;
		float y;
		float z;
	};

	struct Vec4
	{
		Vec4(float posX, float posY, float posZ, float posW)
			: x(posX), y(posY), z(posZ), w(posW)
		{}

		Vec4(Vec2 xy, float posZ, float posW)
			: x(xy.x), y(xy.y), z(posZ), w(posW)
		{}

		Vec4(Vec2 xy, Vec2 zw)
			: x(xy.x), y(xy.y), z(zw.x), w(zw.y)
		{}

		Vec4(Vec3 xyz, float posW)
			: x(xyz.x), y(xyz.y), z(xyz.z), w(posW)
		{}

		Vec4()
			: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{}

		inline friend bool operator==(const Vec4 lhs, const Vec4 rhs)
		{
			return ((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w));
		}

		inline friend bool operator!=(const Vec4 lhs, const Vec4 rhs)
		{
			return !(lhs == rhs);
		}

		inline friend Vec4 operator+(const Vec4 lhs, const Vec4 rhs)
		{
			return Vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
		}

		inline friend Vec4 operator+=(const Vec4 lhs, const Vec4 rhs)
		{
			return lhs + rhs;
		}

		inline friend Vec4 operator-(const Vec4 lhs, const Vec4 rhs)
		{
			return Vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
		}

		inline friend Vec4 operator-=(const Vec4 lhs, const Vec4 rhs)
		{
			return lhs - rhs;
		}

		float x;
		float y;
		float z;
		float w;
	};


	/*Typical Vertex holding Vec3 position(x, y, z), normal(x, y, z) and color(r, g, b)*/
	struct Vertex
	{
		Vertex()
			: position(std::move(Vec3(0.0f, 0.0f, 0.0f))),
			normal(std::move(Vec3(0.0f, 0.0f, 0.0f))),
			color(std::move(Vec3(0.0f, 0.0f, 0.0f)))
		{

		}

		Vertex(Vec3&& vecPosition, Vec3&& vecNormal, Vec3&& vecColor)
			: position(std::move(vecPosition)),
			normal(std::move(vecNormal)),
			color(std::move(vecColor))
		{

		}

		Vertex(Vec3 vecPosition, Vec3 vecNormal, Vec3 vecColor)
			: position(vecPosition), normal(vecNormal), color(vecColor)
		{
		}
		//@TODO: Handle possible errors in the future
		inline SkResult<vk::VertexInputBindingDescription, UndefinedError> getInputBindingDescription(uint32_t bindingIndex = 0, vk::VertexInputRate bindingInputRate = vk::VertexInputRate::eVertex)
		{
			SkResult result(static_cast<vk::VertexInputBindingDescription>(vk::VertexInputBindingDescription()), static_cast<UndefinedError>(UndefinedError::NO_ERROR));

			vk::VertexInputBindingDescription vertexInputBindingDescription = {};
			vertexInputBindingDescription.binding = bindingIndex;
			vertexInputBindingDescription.stride = sizeof(Vertex);
			vertexInputBindingDescription.inputRate = bindingInputRate;

			result.value = std::move(vertexInputBindingDescription);
			return result;
		}

		inline SkResult<std::vector<vk::VertexInputAttributeDescription>, UndefinedError> getInputAttributeDescriptions(uint32_t bindingIndex = 0, vk::Format attributeFormat = vk::Format::eR32G32B32Sfloat)
		{
			std::vector<vk::VertexInputAttributeDescription> value = {};
			UndefinedError error = UndefinedError::NO_ERROR;

			std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptons;
			vertexInputAttributeDescriptons.reserve(3); // 3 as in 3 members (position, normal, color)

			vk::VertexInputAttributeDescription positionAttribute = {};
			positionAttribute.binding = bindingIndex;
			positionAttribute.location = 0;
			positionAttribute.format = vk::Format::eR32G32B32Sfloat;
			positionAttribute.offset = offsetof(Vertex, position);
			vertexInputAttributeDescriptons.emplace_back(positionAttribute);

			vk::VertexInputAttributeDescription normalAttribute = {};
			normalAttribute.binding = bindingIndex;
			normalAttribute.location = 1;
			normalAttribute.format = vk::Format::eR32G32B32Sfloat;
			normalAttribute.offset = offsetof(Vertex, normal);
			vertexInputAttributeDescriptons.emplace_back(normalAttribute);

			vk::VertexInputAttributeDescription colorAttribute = {};
			colorAttribute.binding = bindingIndex;
			colorAttribute.location = 2;
			colorAttribute.format = vk::Format::eR32G32B32Sfloat;
			colorAttribute.offset = offsetof(Vertex, color);
			vertexInputAttributeDescriptons.emplace_back(colorAttribute);

			value = vertexInputAttributeDescriptons;
			return SkResult(value, error);
		}

		Vec3 position;
		Vec3 normal;
		Vec3 color;
	};

	/*Typical Mesh holding a list of Vertices*/
	struct Mesh
	{
		Mesh(std::vector<Vertex> vert)
		{
			vertices = vert;
		}

		Mesh()
		{
			vertices = {};
		}

		//inline SkResult<std::vector<Vertex>, UndefinedError> getVertices()
		//{
		//	SkResult result(static_cast<std::vector<Vertex>>(std::vector<Vertex>()), static_cast<UndefinedError>(UndefinedError::NO_ERROR));
		//
		//	result.value = vertices;
		//	return result;
		//}

		std::vector<Vertex> vertices;
	};

	/*Frame and frametime stuf*/

	/*@brief Calculates the FPS given a number of frames elapsed in delta time
	*
	* @param frames The number of frames elapsed during delta time
	* @param delta The elapsed time
	*
	* @return SkResult(FPS, MathError)
	*/
	inline SkResult<double, MathError> getFramePerSecond(double frames, std::chrono::duration<double> delta) noexcept
	{
		SkResult result(static_cast<double>(0.0f), static_cast<MathError>(MathError::NO_ERROR));

		if (delta.count() == 0)
		{
			result.value = 0;
			result.error = MathError::DIVIDING_BY_ZERO_ERROR;
		}
		else
		{
			result.value = frames / delta.count();
		}

		return result;
	}

	/*@brief Calculates the frametime with a given FPS value
	*
	* @param framePerSecond The given FPS
	*
	* @return SkResult(frametime in miliseconds, MathError)
	*/
	inline SkResult<double, MathError> getFrametime(double framePerSecond) noexcept
	{
		SkResult result(static_cast<double>(0.0f), static_cast<MathError>(MathError::NO_ERROR));

		if (framePerSecond == 0)
		{
			result.value = 0;
			result.error = MathError::DIVIDING_BY_ZERO_ERROR;
		}
		else
		{
			result.value = 1000.0 / framePerSecond;
		}

		return result;
	}

	/*GLFW
	* Main functions concerning init and terminate, window creation.
	*/

	/**
		* @brief Initializes GLFW, must be called before any GLFW operation.
		*
		* @return SkResult(Result from glfwInit(), GLFWError)
		*/
	inline SkResult<uint32_t, GLFWError> initGlfw() noexcept
	{
		SkResult result(static_cast<uint32_t>(glfwInit()), static_cast<GLFWError>(GLFWError::NO_ERROR));

		if (!result.value)
		{
			result.error = std::move(GLFWError::INIT_ERROR);

		}
		else
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}

		return result;
	}

	/**
		* @brief Terminates glfw.
		*
		* @return SkResult(bool signaling if the operation worked or not, GLFWError)
		*/
	inline SkResult<bool, GLFWError> terminateGlfw() noexcept
	{
		SkResult result(static_cast<bool>(true), static_cast<GLFWError>(GLFWError::NO_ERROR));

		glfwTerminate();

		return result;
	}

	/**
		* @brief Creates a GLFW Window, must be called after init().
		*
		* @param width : Sets the width in pixel
		* @param height : Sets the height in pixel
		* @param title : Sets the title of the GLFW window
		* @param resizable : Sets if the window should be resizable or not
		*
		* @return inline SkResult<Pointer to the GLFW window, GLFWError>
		*
		*/
	inline SkResult<GLFWwindow*, GLFWError> createGlfwWindow(uint32_t width, uint32_t height, std::string title, bool resizable) noexcept
	{
		SkResult result(static_cast<GLFWwindow*>(nullptr), static_cast<GLFWError>(GLFWError::NO_ERROR));

		if (resizable)
		{
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		}
		else
		{
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		}

		result.value = glfwCreateWindow(width, height, title.c_str(), 0, nullptr);

		if (result.value == nullptr)
		{
			result.error = std::move(GLFWError::WINDOW_CREATION_ERROR);
		}

		return result;
	}

	inline SkResult<GLFWwindow*, GLFWError> createWindow(uint32_t width, uint32_t height, std::string title, bool resizable) noexcept
	{
		SkResult result(static_cast<GLFWwindow*>(nullptr), static_cast<GLFWError>(GLFWError::NO_ERROR));

		auto initGlfwResult = initGlfw();
		result.error = std::move(affectError(initGlfwResult, result.error));

		auto createGlfwWindowResult = createGlfwWindow(width, height, title, resizable);
		result.error = std::move(affectError(createGlfwWindowResult, result.error));
		result.value = std::move(retLog(createGlfwWindowResult));

		return result;
	}



	/*DEBUG UTILS
	 *Useful functions concerning the debug utils messenger from vulkan.
	 * @TODO : Clean up all that ugly stuff
	 */

	 /*
	 * @brief Override of a vulkan function to print vulkan messages directly to a file. Is the default debugCallback if none are given at instance creation time.
	 */
	inline static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		std::ofstream debugFile;
		debugFile.open("debugCallbackOutput.txt", std::ios::app);

		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			debugFile << "VERB : " << pCallbackData->pMessage << "\n";
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			debugFile << "INFO : " << pCallbackData->pMessage << "\n";
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			debugFile << "WARN : " << pCallbackData->pMessage << "\n";
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			debugFile << "ERRO : " << pCallbackData->pMessage << "\n";
			break;


		}

		return VK_FALSE;
	}

	/*
	* @brief Calls a vulkan function via a function pointer to create the debugUtilsMessenger
	*/
	inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) noexcept
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	/*
	* @brief Calls a vulkan function via a function pointer to destroy the debugUtilsMessenger
	*/
	inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) noexcept
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}

	/*
	* @brief Creates a DebugUtilsMessengerCreateInfo with the basic information including the messageSeverity and MessageType Flags
	*
	* @param debugCallback The debug callback to be used in the the debug utils messenger
	*
	* @return DebugUtilsMessengerCreateInfo
	*/
	inline SkResult<vk::DebugUtilsMessengerCreateInfoEXT, DebugUtilsError> createDebugUtilsMessengerCreateInfo(PFN_vkDebugUtilsMessengerCallbackEXT debugCallback)
	{
		SkResult result(static_cast<vk::DebugUtilsMessengerCreateInfoEXT>(vk::DebugUtilsMessengerCreateInfoEXT{}), static_cast<DebugUtilsError>(DebugUtilsError::NO_ERROR));

		try
		{
			result.value = vk::DebugUtilsMessengerCreateInfoEXT(vk::DebugUtilsMessengerCreateFlagsEXT(),
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
				debugCallback,
				nullptr);
		}
		catch (vk::SystemError err)
		{
			result.error = std::move(DebugUtilsError::DEBUG_CALLBACK_CI_ERROR);
		}

		return result;
	}

	/*@brief Creates a debug utils messenger via CreateDebugUtilsMessengerEXT()
	*
	* @param instance The vulkan instance used to create the debug utils messenger
	* @param debugUtilsMessengerCreateInfo The create info to be used for creating the debug utils messenger
	*
	* @return SkResult(boolean indicating if the operation worked(true) or not(false), DebugUtilsError)
	*/
	inline SkResult<bool, DebugUtilsError> createDebugUtilsMessenger(vk::Instance instance, VkDebugUtilsMessengerCreateInfoEXT* debugUtilsMessengerCreateInfo)
	{
		SkResult result(static_cast<bool>(true), static_cast<DebugUtilsError>(DebugUtilsError::NO_ERROR));

		try
		{
			CreateDebugUtilsMessengerEXT(instance, debugUtilsMessengerCreateInfo, nullptr, nullptr);
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = DebugUtilsError::DEBUG_CALLBACK_CREATION_ERROR;
		}

		return result;
	}

	/*Instance*/

	/*
	* @brief Gets the required extensions from glfwGetRequiredInstanceExtensions()
	*
	* @param validationEnabled Are the validation layers enabled or not, if they are then the DebugUtils extension is added to the list of required extensions
	*
	* @return SkResult(List of extension names, InstanceError)
	*/
	inline SkResult<std::vector<const char*>, InstanceError> getRequiredInstanceExtensions(bool validationEnabled) noexcept
	{
		SkResult result(static_cast<std::vector<const char*>>(std::vector<const char*>()), static_cast<InstanceError>(InstanceError::NO_ERROR));

		uint32_t glfwExtensionCount = 0;
		const char** ppGlfwExtensions = nullptr;

		ppGlfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		if (glfwExtensionCount == 0 || ppGlfwExtensions == nullptr)
		{
			result.error = InstanceError::GET_REQUIRED_INSTANCE_EXTENSIONS_ERROR;
		}

		auto extensions = std::vector<const char*>(ppGlfwExtensions, ppGlfwExtensions + glfwExtensionCount);
		if (validationEnabled)
		{
			extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		result.value = std::move(extensions);
		return result;
	}

	/*
	* @brief Creates a vulkan ApplicationInfo.
	*
	* Any error is stored in the instanceError variable
	*
	* @param pApplicationName Application name
	* @param pEngineName Engine name
	*
	* @return SkResult(application info, InstanceError)
	*/
	inline SkResult<vk::ApplicationInfo, InstanceError> createApplicationInfo(const char* pApplicationName, const char* pEngineName) noexcept
	{
		SkResult result(static_cast<vk::ApplicationInfo>(vk::ApplicationInfo{}), static_cast<InstanceError>(InstanceError::NO_ERROR));

		try
		{
			result.value = vk::ApplicationInfo(pApplicationName,
				VK_MAKE_VERSION(1, 0, 0),
				pEngineName,
				VK_MAKE_VERSION(1, 0, 0),
				VK_API_VERSION_1_0);
		}
		catch (vk::SystemError err)
		{
			result.error = InstanceError::APPLICATION_INFO_CREATION_ERROR;
		}

		return result;
	}

	/*
	* @brief Creates a vulkan Instance.
	*
	* Any error is stored in the instanceError variable
	*
	* @param applicationInfo A vulkan applicationInfo
	* @param extensions List of extension names (default is an empty vector of char*)
	* @param validationLayers List of validation layer names (default is an empty vector of char*)
	* @param debugCallback debugCallback (default is nullptr)
	*
	* @return SkResult(vulkan instance, InstanceError)
	*/
	inline SkResult<vk::Instance, InstanceError> createInstance(vk::ApplicationInfo applicationInfo, std::vector<const char*> extensions = std::vector<const char*>(), std::vector<const char*> validationLayers = std::vector<const char*>(),
		PFN_vkDebugUtilsMessengerCallbackEXT debugUtilsMessenger = nullptr)
	{
		SkResult result(static_cast<vk::Instance>(vk::Instance(nullptr)), static_cast<InstanceError>(InstanceError::NO_ERROR));

		if (extensions.empty())
		{
			if (!validationLayers.empty())
			{
				auto getRequiredExtensionsResult = getRequiredInstanceExtensions(true);
				result.error = std::move(affectError(getRequiredExtensionsResult, result.error));

				extensions = std::move(retLog(getRequiredExtensionsResult));
			}
			else
			{
				auto getRequiredExtensionsResult = getRequiredInstanceExtensions(false);
				result.error = std::move(affectError(getRequiredExtensionsResult, result.error));

				extensions = std::move(retLog(getRequiredExtensionsResult));
			}
		}

		auto instanceCreateInfo = std::move(vk::InstanceCreateInfo(vk::InstanceCreateFlags(),
			&applicationInfo,
			static_cast<uint32_t>(validationLayers.size()),
			validationLayers.data(),
			static_cast<uint32_t>(extensions.size()),
			extensions.data()));


		vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {};

		auto createDebugUtilsMessengerCreateInfoResult = std::move(createDebugUtilsMessengerCreateInfo(debugCallback));
		debugUtilsMessengerCreateInfo = std::move(retLog(createDebugUtilsMessengerCreateInfoResult));

		//Doesn't work for now
		//if (debugCallback == nullptr && !validationLayers.empty())
		//{
		//	auto createDebugUtilsMessengerCreateInfoResult = createDebugUtilsMessengerCreateInfo(debugCallback);
		//	debugUtilsMessengerCreateInfo = retLog(createDebugUtilsMessengerCreateInfoResult);
		//
		//	debugUtilsMessengerCreateInfo.pfnUserCallback = debugCallback;
		//}
		//else if (debugCallback != nullptr && !validationLayers.empty())
		//{
		//	auto createDebugUtilsMessengerCreateInfoResult = createDebugUtilsMessengerCreateInfo(debugUtilsMessenger);
		//	debugUtilsMessengerCreateInfo = retLog(createDebugUtilsMessengerCreateInfoResult);
		//
		//	debugUtilsMessengerCreateInfo.pfnUserCallback = debugUtilsMessenger;
		//}

		debugUtilsMessengerCreateInfo.pfnUserCallback = debugCallback;
		instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugUtilsMessengerCreateInfo;

		vk::Instance instance;
		try
		{
			instance = std::move(vk::createInstance(instanceCreateInfo));
		}
		catch (vk::SystemError err)
		{
			result.error = InstanceError::INSTANCE_CREATION_ERROR;
		}

		result.value = std::move(instance);
		return result;
	}

	inline SkResult<vk::Instance, InstanceError> createInstance(const char* pApplicationName, const char* pEngineName, std::vector<const char*> extensions = std::vector<const char*>(),
		std::vector<const char*> validationLayers = std::vector<const char*>(), PFN_vkDebugUtilsMessengerCallbackEXT debugUtilsMessenger = nullptr) noexcept
	{
		SkResult result(static_cast<vk::Instance>(vk::Instance(nullptr)), static_cast<InstanceError>(InstanceError::NO_ERROR));

		auto createApplicationInfoResult = createApplicationInfo(pApplicationName, pEngineName);
		result.error = std::move(affectError(createApplicationInfoResult, result.error));
		vk::ApplicationInfo applicationInfo = std::move(retLog(createApplicationInfoResult));

		auto createInstanceResult = createInstance(applicationInfo, extensions, validationLayers, debugUtilsMessenger);
		result.error = std::move(affectError(createInstanceResult, result.error));
		vk::Instance instance = std::move(retLog(createInstanceResult));

		result.value = std::move(instance);
		return result;
	}

	/*
	* @brief Creates a vulkan surface using glfwCreateWindowSurface()
	*
	* Any error is stored in the instanceError variable
	*
	* @param instance A vulkan instance
	* @param pWindow pointer to the GLFW window created with createGLFWWindow()
	*
	* @return SkResult(
	*/
	inline SkResult<vk::SurfaceKHR, InstanceError> createGLFWWindowSurface(vk::Instance instance, GLFWwindow* pWindow) noexcept
	{
		SkResult result(static_cast<vk::SurfaceKHR>(vk::SurfaceKHR(nullptr)), static_cast<InstanceError>(InstanceError::NO_ERROR));

		VkSurfaceKHR rawSurface;
		VkResult res = std::move(glfwCreateWindowSurface(instance, pWindow, nullptr, &rawSurface));

		if (res != VK_SUCCESS || rawSurface == VK_NULL_HANDLE)
		{
			result.error = InstanceError::SURFACE_CREATION_ERROR;
		}

		result.value = std::move(rawSurface);
		return result;
	}

	/*PHYSICAL DEVICE*/


	/*@brief Returns all the available physical devices on the machine
	*
	* @param instance a vulkan instance
	*
	* @return SkResult(vector of physical devices, PhysicalDeviceError)
	*/
	inline SkResult<std::vector<vk::PhysicalDevice>, PhysicalDeviceError> getAvailablePhysicalDevices(vk::Instance instance) noexcept
	{
		SkResult result(static_cast<std::vector<vk::PhysicalDevice>>(std::vector<vk::PhysicalDevice>()), static_cast<PhysicalDeviceError>(PhysicalDeviceError::NO_ERROR));

		auto availablePhysicalDevices = instance.enumeratePhysicalDevices();

		if (availablePhysicalDevices.size() == 0)
		{
			result.error = PhysicalDeviceError::NO_PHYSICAL_DEVICE_FOUND;
		}

		result.value = std::move(availablePhysicalDevices);
		return result;
	}

	/*@brief Reserved for later use
	*
	* @param physicalDevice a vulkan physicaldevice
	*
	* @return boolean indicating if a given vulkan device is suitable(true) or not(false)
	*/
	constexpr inline bool isDeviceSuitable(vk::PhysicalDevice physicalDevice) noexcept
	{
		return true;
	}

	/* @brief Returns all of the suitable vulkan physical devices filtered through isDeviceSuitable()
	 *
	 * @param availablePhysicalDevices vector of the available vulkan physical devices (getAvailablePhysicalDevices())
	 *
	 * @return SkResult(Vector of physical devices, PhysicalDeviceError)
	 */
	inline SkResult<std::vector<vk::PhysicalDevice>, PhysicalDeviceError> getSuitablePhysicalDevices(std::vector<vk::PhysicalDevice> availablePhysicalDevices)
	{
		SkResult result(static_cast<std::vector<vk::PhysicalDevice>>(std::vector<vk::PhysicalDevice>()), static_cast<PhysicalDeviceError>(PhysicalDeviceError::NO_ERROR));

		std::vector<vk::PhysicalDevice> suitablePhysicalDevices;
		suitablePhysicalDevices.reserve(availablePhysicalDevices.size());
		for (int i = 0; i < availablePhysicalDevices.size(); i++)
		{
			if (isDeviceSuitable(availablePhysicalDevices[i]))
			{
				suitablePhysicalDevices.emplace_back(availablePhysicalDevices[i]);
			}
		}

		result.value = std::move(suitablePhysicalDevices);
		return result;
	}

	/* @brief Returns the best vulkan physical device filtered through a score system
	 *
	 * @param suitablePhysicalDevices vector of the suitable vulkan physical devices (getSuitablePhysicalDevices())
	 *
	 * @return SkResult(Vulkan physical device, PhysicalDeviceError)
	 */
	inline SkResult<vk::PhysicalDevice, PhysicalDeviceError> getBestPhysicalDeviceByScore(std::vector<vk::PhysicalDevice> suitablePhysicalDevices)
	{
		SkResult result(static_cast<vk::PhysicalDevice>(vk::PhysicalDevice(nullptr)), static_cast<PhysicalDeviceError>(PhysicalDeviceError::NO_ERROR));

		int i = -1;
		int index = -1;
		uint32_t hiscore = 0;

		for (const auto& physicalDevice : suitablePhysicalDevices)
		{
			i += 1;
			uint32_t score = 0;

			auto physicalDeviceProperties = physicalDevice.getProperties();
			auto physicalDeviceFeatures = physicalDevice.getFeatures();

			// Discrete GPUs have a significant performance advantage
			if (physicalDeviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
			{
				score += 1000;
			}

			// Maximum possible size of textures affects graphics quality
			score += physicalDeviceProperties.limits.maxImageDimension2D;

			// Application can't function without geometry shaders
			if (!physicalDeviceFeatures.geometryShader)
			{
				score = 0;
			}

			if (score > hiscore) { hiscore = score; index = i; }
		}

		if (suitablePhysicalDevices[index] != vk::PhysicalDevice(nullptr))
		{
			result.value = suitablePhysicalDevices[index];
		}
		else
		{
			result.error = PhysicalDeviceError::NO_SUITABLE_PHYSICAL_DEVICE_FOUND;
		}


		return result;
	}

	/*@brief Returns the best suitable vulkan physical device, if there are more than 1 suitable physical device, they are filtered through getBestPhysicalDeviceByScore().
	*
	* @param suitablePhysicalDevices vector of suitable vulkan physical devices
	*
	* @return SkResult(vulkan physical device, PhysicalDeviceError)
	*/
	inline SkResult<vk::PhysicalDevice, PhysicalDeviceError> getBestPhysicalDevice(std::vector<vk::PhysicalDevice> suitablePhysicalDevices)
	{
		SkResult result(static_cast<vk::PhysicalDevice>(vk::PhysicalDevice(nullptr)), static_cast<PhysicalDeviceError>(PhysicalDeviceError::NO_ERROR));

		if (suitablePhysicalDevices.size() == 0)
		{
			result.error = PhysicalDeviceError::NO_SUITABLE_PHYSICAL_DEVICE_FOUND;
		}

		else if (suitablePhysicalDevices.size() == 1)
		{
			result.value = std::move(suitablePhysicalDevices[0]);
		}

		else
		{
			auto bestPhysicalResult = getBestPhysicalDeviceByScore(suitablePhysicalDevices);

			result.value = retLog(bestPhysicalResult);
			result.error = affectError(bestPhysicalResult, result.error);
		}

		return result;
	}

	/*@brief Sorts through all available physical devices and returns the best (getAvailablePhysicalDevices() -> getSuitablePhysicalDevices() -> getBestPhysicalDevice())
	*
	* @param instance The vulkan instance used to get the available devices
	*
	* @return SkResult(vulkan physical device, PhysicalDeviceError)
	*/
	inline SkResult<vk::PhysicalDevice, PhysicalDeviceError> getPhysicalDevice(vk::Instance instance)
	{
		SkResult result(static_cast<vk::PhysicalDevice>(vk::PhysicalDevice(nullptr)), static_cast<PhysicalDeviceError>(PhysicalDeviceError::NO_ERROR));

		auto getAvailablePhysicalDevicesResult = getAvailablePhysicalDevices(instance);
		result.error = std::move(affectError(getAvailablePhysicalDevicesResult, result.error));
		std::vector<vk::PhysicalDevice> availablePhysicalDevices = std::move(retLog(getAvailablePhysicalDevicesResult));

		auto getSuitablePhysicalDevicesResult = getSuitablePhysicalDevices(availablePhysicalDevices);
		result.error = std::move(affectError(getSuitablePhysicalDevicesResult, result.error));
		std::vector<vk::PhysicalDevice> suitablePhysicalDevices = std::move(retLog(getSuitablePhysicalDevicesResult));

		auto getBestPhysicalDeviceResult = getBestPhysicalDevice(suitablePhysicalDevices);
		result.error = std::move(affectError(getBestPhysicalDeviceResult, result.error));
		vk::PhysicalDevice physicalDevice = std::move(retLog(getBestPhysicalDeviceResult));

		result.value = std::move(physicalDevice);
		return result;
	}

	/*@brief Returns a vector of queue family indexes with the following information :
	* In the vector, index 0 = generalFamily, 1 = graphicsFamily, 2 = presentFamily, 3 = computeFamily, 4 = transferFamily, 5 = debug/tmp.
	* (See QueueFamilyType enum)
	*
	* @param physicalDevice a vulkan physical device
	* @param surface a vulkan surface
	*
	* @return SkResult(vector of queue family indexes, PhysicalDeviceError)
	*/
	inline SkResult<std::vector<uint32_t>, PhysicalDeviceError> getQueueFamilyIndexes(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
	{
		SkResult result(static_cast<std::vector<uint32_t>>(std::vector<uint32_t>()), static_cast<PhysicalDeviceError>(PhysicalDeviceError::NO_ERROR));

		uint32_t uint32_tMax = std::move(std::numeric_limits<uint32_t>::max());
		std::vector<uint32_t> queueFamilyIndexes = { uint32_tMax, uint32_tMax, uint32_tMax, uint32_tMax, uint32_tMax, uint32_tMax };

		auto availableQueueFamilies = physicalDevice.getQueueFamilyProperties();

		uint32_t i = 0;
		for (const auto& queueFamily : availableQueueFamilies)
		{
			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics &&
				queueFamily.queueFlags & vk::QueueFlagBits::eCompute &&
				physicalDevice.getSurfaceSupportKHR(i, surface))
			{
				queueFamilyIndexes[static_cast<uint32_t>(QueueFamilyType::GENERAL)] = i;
			}

			i++;
		}

		uint32_t j = 0;
		for (const auto& queueFamily : availableQueueFamilies)
		{
			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				queueFamilyIndexes[static_cast<uint32_t>(QueueFamilyType::GRAPHICS)] = j;
			}

			if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer &&
				queueFamily.queueFlags & vk::QueueFlagBits::eCompute &&
				!(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics))
			{
				queueFamilyIndexes[static_cast<uint32_t>(QueueFamilyType::COMPUTE)] = j;
			}


			if (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer &&
				!(queueFamily.queueFlags & vk::QueueFlagBits::eCompute) &&
				!(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics))
			{
				queueFamilyIndexes[static_cast<uint32_t>(QueueFamilyType::TRANSFER)] = j;
			}

			if (physicalDevice.getSurfaceSupportKHR(j, surface))
			{
				queueFamilyIndexes[static_cast<uint32_t>(QueueFamilyType::PRESENT)] = j;
			}

			j++;
		}

		if (queueFamilyIndexes[static_cast<uint32_t>(QueueFamilyType::GENERAL)] == uint32_tMax ||
			queueFamilyIndexes[static_cast<uint32_t>(QueueFamilyType::GRAPHICS)] == uint32_tMax)
		{
			result.error = PhysicalDeviceError::NO_SUITABLE_QUEUES_FOUND;
		}

		result.value = std::move(queueFamilyIndexes);
		return result;
	}

	/*@brief Returns a vector of "concentrated" queue family indexes,
	* meaning that duplicate indexes are removed and that only known correct indexes are left in the vector (no std::numeric_limits<uint32_t>::max(), nor debug/tmp)
	*
	* @param queueFamilyIndexes a vector of queue family indexes
	*
	* @return SkResult(vector of concentrated queue family indexes, PhysicalDeviceError)
	*/
	inline SkResult<std::vector<uint32_t>, QueueError> concentrateQueueFamilyIndexes(std::vector<uint32_t> queueFamilyIndexes)
	{
		SkResult result(static_cast<std::vector<uint32_t>>(std::vector<uint32_t>()), static_cast<QueueError>(QueueError::NO_ERROR));

		std::vector<uint32_t> concentratedQueueFamilyIndexes(queueFamilyIndexes.begin(), queueFamilyIndexes.end());
		for (int k = 0; k < concentratedQueueFamilyIndexes.size(); k++)
		{
			if (concentratedQueueFamilyIndexes[k] == std::numeric_limits<uint32_t>::max())
			{
				concentratedQueueFamilyIndexes.erase(concentratedQueueFamilyIndexes.begin() + k);
			}
		}
		for (int i = 0; i < concentratedQueueFamilyIndexes.size(); i++)
		{
			for (int j = 0; j < concentratedQueueFamilyIndexes.size(); j++)
			{
				if ((i != j) && (concentratedQueueFamilyIndexes[i] == concentratedQueueFamilyIndexes[j]))
				{
					concentratedQueueFamilyIndexes.erase(concentratedQueueFamilyIndexes.begin() + i);
				}
			}
		}

		if (concentratedQueueFamilyIndexes.size() == 0)
		{
			result.error = QueueError::EMPTY_CONCENTRATED_QUEUE_VECTOR;
		}

		result.value = std::move(concentratedQueueFamilyIndexes);
		return result;
	}

	/*@brief Returns the best available present mode, the chosen present mode or the other preferred modes if it's not available :
	* Mailbox > Fifo > FifoRelaxed > Immediate
	*
	* @param physicalDevice the vulkan physical device used from which to get the surface present modeds
	* @param surface the vulkan surface used to get present modes from
	* @param presentMode the chosen vulkan present mode
	*
	* @return SkResult(best available present mode, PhysicalDeviceError)
	*/
	inline SkResult<vk::PresentModeKHR, PhysicalDeviceError> getBestAvailablePresentMode(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::PresentModeKHR presentMode)
	{
		SkResult result(static_cast<vk::PresentModeKHR>(vk::PresentModeKHR::eImmediate), static_cast<PhysicalDeviceError>(PhysicalDeviceError::NO_ERROR)); //Could not find an acceptable default value for PresentModeKHR

		std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
		if (presentModes.size() == 0)
		{
			result.error = PhysicalDeviceError::NO_PRESENT_MODES;
		}

		bool isChoiceAvailable = false;

		bool isImmediateAvailable = false;
		bool isFifoAvailable = false;
		bool isFifoRelaxedAvailable = false;
		bool isMailboxAvailable = false;

		//Checks if the chosen present mode is available, otherwise check if every present mode is available or not
		for (const auto& mode : presentModes)
		{
			if (mode == presentMode)
			{
				isChoiceAvailable = true;
				break;
			}

			if (mode == vk::PresentModeKHR::eImmediate)
			{
				isImmediateAvailable = true;
			}

			if (mode == vk::PresentModeKHR::eFifo)
			{
				isFifoAvailable = true;
			}

			if (mode == vk::PresentModeKHR::eFifoRelaxed)
			{
				isFifoRelaxedAvailable = true;
			}

			if (mode == vk::PresentModeKHR::eMailbox)
			{
				isMailboxAvailable = true;
			}
		}

		//Picks the chosen present mode if it's available, otherwise go through preferred present modes one by one
		if (isChoiceAvailable)
		{
			result.value = std::move(presentMode);
		}
		else
		{
			if (isMailboxAvailable)
			{
				result.value = vk::PresentModeKHR::eMailbox;
			}

			else if (isFifoAvailable)
			{
				result.value = vk::PresentModeKHR::eFifo;
			}

			else if (isFifoRelaxedAvailable)
			{
				result.value = vk::PresentModeKHR::eFifo;
			}

			else
			{
				result.value = vk::PresentModeKHR::eImmediate;
			}
		}

		return result;
	}

	/*@brief Returns the best available surface format from a given physical device for a given surface
	*
	* @param physicalDevice the vulkan physical device used to get the surface format from
	* @param surface the vulkan surface used to get the surface format
	*
	* @return SkResult(best available surface format, PhysicalDeviceError)
	*/
	inline SkResult<vk::SurfaceFormatKHR, PhysicalDeviceError> getBestAvailableSurfaceFormat(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
	{
		SkResult result(static_cast<vk::SurfaceFormatKHR>(vk::SurfaceFormatKHR()), static_cast<PhysicalDeviceError>(PhysicalDeviceError::NO_ERROR)); // No default value for vk::SurfaceFormatKHR

		std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);

		if (surfaceFormats.size() == 0)
		{
			result.error = PhysicalDeviceError::NO_SURFACE_FORMATS;
		}

		if (surfaceFormats.size() == 1 && surfaceFormats[0].format == vk::Format::eUndefined)
		{
			result.error = PhysicalDeviceError::UNDEFINED_FORMAT;

			result.value = std::move(vk::SurfaceFormatKHR{ vk::Format::eB8G8R8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear });
		}

		else if (surfaceFormats.size() == 1)
		{
			result.value = std::move(surfaceFormats[0]);
		}

		else
		{
			for (const auto& surfaceFormat : surfaceFormats)
			{
				if (surfaceFormat.format == vk::Format::eB8G8R8A8Unorm && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				{
					result.value = std::move(surfaceFormat);
				}
			}
		}

		return result;
	}

	/*@brief Returns the swapchain 2D extent for a given physical device, surface and GLFW window
	*
	* @param physicalDevice the vulkan physical device from which to get the surface capabilities
	* @param surface the vulkan surface used to get its capabilities
	* @param pWindow the glfw window from which the framebuffer size is returned
	*
	* @return SkResult(2D extent of the swapchain, PhysicalDeviceError)
	*/
	inline SkResult<vk::Extent2D, PhysicalDeviceError> getSwapchainExtent(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, GLFWwindow* pWindow)
	{
		SkResult result(static_cast<vk::Extent2D>(vk::Extent2D()), static_cast<PhysicalDeviceError>(PhysicalDeviceError::NO_ERROR));

		vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);

		if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			result.value = std::move(surfaceCapabilities.currentExtent);
		}
		else
		{
			int width, height;
			glfwGetFramebufferSize(pWindow, &width, &height);

			if (width == NULL && height == NULL)
			{
				result.error = PhysicalDeviceError::GLFW_FRAMEBUFFER_SIZE_ERROR;
			}

			vk::Extent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

			actualExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, actualExtent.height));

			result.value = std::move(actualExtent);
		}

		return result;
	}

	inline SkResult<std::string, PhysicalDeviceError> getMemoryInformations(vk::PhysicalDevice physicalDevice)
	{
		SkResult result(static_cast<std::string>(std::string()), static_cast<PhysicalDeviceError>(PhysicalDeviceError::NO_ERROR));

		uint64_t bytesToMbConversion = 1048576;

		vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

		std::string memoryInformations = std::string("");
		memoryInformations.append("Heap count : " + std::to_string(memoryProperties.memoryHeapCount) + '\t');
		memoryInformations.append("Type count : " + std::to_string(memoryProperties.memoryTypeCount) + '\n');

		for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; i++)
		{
			uint32_t memoryTypeRelativeIndex = 0;

			memoryInformations.append("Heap [" + std::to_string(i) + "] ");

			if (memoryProperties.memoryHeaps[i].flags == vk::MemoryHeapFlagBits::eDeviceLocal)
			{
				memoryInformations.append("(DEVICE MEMORY)");
			}

			memoryInformations.append(" : \n");

			memoryInformations.append("\tSize : " + std::to_string(memoryProperties.memoryHeaps[i].size) + " bytes (" + std::to_string(memoryProperties.memoryHeaps[i].size / bytesToMbConversion) + " MB) \n");

			for (uint32_t j = 0; j < memoryProperties.memoryTypeCount; j++)
			{
				if (memoryProperties.memoryTypes[j].heapIndex == i)
				{
					memoryInformations.append("\tMemory type [" + std::to_string(memoryTypeRelativeIndex) + "] : ");
					memoryInformations.append(vk::to_string(memoryProperties.memoryTypes[j].propertyFlags));
					memoryInformations.append("\n");

					memoryTypeRelativeIndex++;
				}
			}
		}

		result.value = std::move(memoryInformations);
		return result;
	}

	inline SkResult<uint32_t, PhysicalDeviceError> getHeapIndexByTypeIndex(vk::PhysicalDevice physicalDevice, uint32_t memoryTypeIndex)
	{
		uint32_t value = std::numeric_limits<uint32_t>::max();
		PhysicalDeviceError error = PhysicalDeviceError::NO_ERROR;

		vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

		uint32_t heapIndex = std::numeric_limits<uint32_t>::max();
		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			if (i == memoryTypeIndex)
			{
				heapIndex = memoryProperties.memoryTypes[i].heapIndex;
			}
		}

		value = heapIndex;
		return SkResult(value, error);
	}

	/*LOGICAL DEVICE*/

	/*@brief Returns a vulkan device from a given vulkan physical device
	*
	* @param physicalDevice a vulkan physical device
	* @param queueFamilyIndexes a vector of queue family indexes to be included in the device creation
	* @param deviceExtensions a vector of extension (names) to be enabled on the device
	* @param physicalDeviceFeatures vulkan physical device features to be enabled on the dedvice
	* @param specificQueueFamilyIndex the index of the queue family to be enabled on the device (default is -1), -1 value means all the indexes from queueFamilyIndexes are used for device creation
	*/
	inline SkResult<vk::Device, DeviceError> createDevice(vk::PhysicalDevice physicalDevice, std::vector<uint32_t> queueFamilyIndexes, std::vector<const char*> deviceExtensions,
		vk::PhysicalDeviceFeatures physicalDeviceFeatures = vk::PhysicalDeviceFeatures{}, int32_t specificQueueFamilyIndex = -1)
	{
		SkResult result(static_cast<vk::Device>(vk::Device(nullptr)), static_cast<DeviceError>(DeviceError::NO_ERROR));

		//DeviceQueueCreateInfos
		auto concentrateQueueFamilyIndexesResult = concentrateQueueFamilyIndexes(queueFamilyIndexes);
		std::vector<uint32_t> concentratedQueueFamilyIndexes = std::move(retLog(concentrateQueueFamilyIndexesResult));

		std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos;
		deviceQueueCreateInfos.reserve(concentratedQueueFamilyIndexes.size());

		float queuePriority = 1.0f;

		if (specificQueueFamilyIndex == -1)
		{
			for (const auto queueFamilyIndex : concentratedQueueFamilyIndexes)
			{
				uint32_t index = std::move(static_cast<uint32_t>(queueFamilyIndex));
				deviceQueueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags(), index, 1, &(queuePriority));
			}
		}

		else
		{
			if ((specificQueueFamilyIndex > queueFamilyIndexes.size() && specificQueueFamilyIndex < queueFamilyIndexes.size()))
			{
				result.error = DeviceError::WRONG_QUEUE_FAMILY_INDEX;
			}

			else
			{
				uint32_t index = std::move(static_cast<uint32_t>(queueFamilyIndexes[specificQueueFamilyIndex]));
				deviceQueueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags(), index, 1, &(queuePriority));
			}
		}

		//DeviceCreateInfo
		vk::DeviceCreateInfo deviceCreateInfo = std::move(vk::DeviceCreateInfo(vk::DeviceCreateFlags(), static_cast<uint32_t>(deviceQueueCreateInfos.size()), deviceQueueCreateInfos.data()));

		if (deviceExtensions.size() == 0)
		{
			result.error = DeviceError::DEVICE_EXTENSIONS_EMPTY;
		}

		physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;

		deviceCreateInfo.pEnabledFeatures = &(physicalDeviceFeatures);
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

		vk::Device device;

		try
		{
			device = std::move(physicalDevice.createDevice(deviceCreateInfo));
		}
		catch (vk::SystemError err)
		{
			result.error = DeviceError::DEVICE_CREATION_ERROR;
		}

		result.value = std::move(device);
		return result;
	}

	/*@brief Destroys a given vulkan device
	*
	* @param device the vulkan device to be destroyed
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), DeviceError)
	*/
	inline SkResult<bool, DeviceError> destroyDevice(vk::Device device)
	{
		SkResult result(static_cast<bool>(true), static_cast<DeviceError>(DeviceError::NO_ERROR));

		try
		{
			device.destroy();
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = DeviceError::DEVICE_DESTRUCTION_ERROR;
		}

		return result;
	}

	/*@brief Returns a vulkan queue from a given queue family index and a given queue index
	*
	* @param device a vulkan device
	* @param queueFamilyIndex the index of the queueFamily from which the queue will stem from
	* @param queueIndex the index of the queue from the queueFamily
	*/
	inline SkResult<vk::Queue, DeviceError> getQueue(vk::Device device, uint32_t queueFamilyIndex, uint32_t queueIndex)
	{
		SkResult result(static_cast<vk::Queue>(vk::Queue(nullptr)), static_cast<DeviceError>(DeviceError::NO_ERROR));

		try
		{
			result.value = device.getQueue(queueFamilyIndex, queueIndex);
		}
		catch (vk::SystemError err)
		{
			result.error = DeviceError::QUEUE_CREATION_ERROR;
		}

		return result;
	}

	/*QUEUE*/

	/*@brief Submits a given command buffer from a given queue, infos such as semaphores and fences must be included to ensure synchronization
	*
	* @param queue the queue which to submit from
	* @param commandBuffer vector of command buffers holding the commands to be submitted
	* @param waitSemaphore vector of semaphores on which to wait before submitting
	* @param signalSemaphore vector of semaphores to signal after submitting
	* @param renderFence the fence to signal after submitting
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), DeviceError)
	*/
	inline SkResult<bool, DeviceError> queueSubmit(vk::Queue queue, std::vector<vk::CommandBuffer> commandBuffer, std::vector<vk::Semaphore> waitSemaphores, std::vector<vk::Semaphore> signalSemaphore, vk::Fence renderFence)
	{
		SkResult result(static_cast<bool>(true), static_cast<DeviceError>(DeviceError::NO_ERROR));

		vk::SubmitInfo submitInfo = {};

		vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;

		submitInfo.pWaitDstStageMask = &waitStage;

		submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
		submitInfo.pWaitSemaphores = waitSemaphores.data();

		submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphore.size());
		submitInfo.pSignalSemaphores = signalSemaphore.data();

		submitInfo.commandBufferCount = static_cast<uint32_t>(commandBuffer.size());
		submitInfo.pCommandBuffers = commandBuffer.data();

		try
		{
			queue.submit(submitInfo, renderFence);
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = DeviceError::QUEUE_SUBMIT_ERROR;
		}

		return result;
	}

	/*@brief Submits a given command buffer from a given queue, infos such as semaphores and fences must be included to ensure synchronization
	*
	* @param queue the queue which to submit from
	* @param commandBuffer the command buffer holding the commands to be submitted
	* @param waitSemaphore the semaphore on which to wait before submitting
	* @param signalSemaphore the semaphore to signal after submitting
	* @param renderFence the fence to signal after submitting
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), DeviceError)
	*/
	inline SkResult<bool, DeviceError> queueSubmit(vk::Queue queue, vk::CommandBuffer commandBuffer, vk::Semaphore waitSemaphores, vk::Semaphore signalSemaphore, vk::Fence renderFence)
	{
		SkResult result(static_cast<bool>(true), static_cast<DeviceError>(DeviceError::NO_ERROR));

		vk::SubmitInfo submitInfo = {};

		vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;

		submitInfo.pWaitDstStageMask = &waitStage;

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &waitSemaphores;

		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &signalSemaphore;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		try
		{
			queue.submit(submitInfo, renderFence);
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = DeviceError::QUEUE_SUBMIT_ERROR;
		}

		return result;
	}

	/*@brief Presents from a given queue a given swapchain image using a given swapchain image index, semaphore must be included to ensure synchronization
	*
	* @param queue a vulkan queue to present from
	* @param swapchain a vulkan swapchain from which the presented image stems from
	* @param waitSemaphore the semaphore on which to wait before presenting
	* @param swapchainImageIndex the index of the image to be rendered from the swapchain
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), DeviceError)
	*/
	inline SkResult<bool, DeviceError> queuePresent(vk::Queue queue, vk::SwapchainKHR swapchain, vk::Semaphore waitSemaphore, uint32_t swapchainImageIndex)
	{
		SkResult result(static_cast<bool>(true), static_cast<DeviceError>(DeviceError::NO_ERROR));

		vk::PresentInfoKHR presentInfo = {};

		presentInfo.pSwapchains = &swapchain;
		presentInfo.swapchainCount = 1;

		presentInfo.pWaitSemaphores = &waitSemaphore;
		presentInfo.waitSemaphoreCount = 1;

		presentInfo.pImageIndices = &swapchainImageIndex;

		try
		{
			auto res = queue.presentKHR(presentInfo);
			if (res != vk::Result::eSuccess)
			{
				result.error = DeviceError::QUEUE_PRESENT_ERROR;
			}
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = DeviceError::QUEUE_PRESENT_ERROR;
		}

		return result;
	}

	/*Swapchain*/

	/*@brief (No the recommended function, please refer to the createSwapchain() using a present mode, surface format and 2D extent as parameters)
	* Creates a vulkan swapchain from a given vulkan device using a given vulkan physicalDevice, surface, present mode, a GLFW window and a vector of allowed queue family indices
	*
	* @param physicalDevice the vulkan physical device used to query surface capabilities
	* @param device the vulkan device from which the swapchain is created
	* @param surface the vulkan surface used to get its capabilities
	* @param pWindow the GLFW window used to query the swapchain 2D extent
	* @param chosenPresentMode the preferred present mode to use over the other is it's available
	* @param allowedQueueIndices a vector of all the allowed queue indices for swapchain creation
	*
	* @return SkResult(created swapchain, SwapchainError)
	*/
	inline SkResult<vk::SwapchainKHR, SwapchainError> createSwapchain(vk::PhysicalDevice physicalDevice, vk::Device device, vk::SurfaceKHR surface, GLFWwindow* pWindow, vk::PresentModeKHR chosenPresentMode,
		std::vector<uint32_t> allowedQueueIndices)
	{
		SkResult result(static_cast<vk::SwapchainKHR>(vk::SwapchainKHR(nullptr)), static_cast<SwapchainError>(SwapchainError::NO_ERROR));

		SkResult<std::vector<uint32_t>, QueueError> concentrateQueueFamilyIndexesResult = concentrateQueueFamilyIndexes(allowedQueueIndices);
		//incompatible enum class error so no affectError() here
		allowedQueueIndices = std::move(retLog(concentrateQueueFamilyIndexesResult));


		vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
		uint32_t swapchainImageCount = std::move(surfaceCapabilities.minImageCount + 1);
		if (surfaceCapabilities.maxImageCount > 0 && swapchainImageCount > surfaceCapabilities.maxImageCount)
		{
			swapchainImageCount = surfaceCapabilities.maxImageCount;
		}

		vk::PresentModeKHR presentMode = std::move(retLog(getBestAvailablePresentMode(physicalDevice, surface, chosenPresentMode)));
		vk::SurfaceFormatKHR surfaceFormat = std::move(retLog(getBestAvailableSurfaceFormat(physicalDevice, surface)));
		vk::Extent2D extent = std::move(retLog(getSwapchainExtent(physicalDevice, surface, pWindow)));

		vk::SwapchainCreateInfoKHR swapchainCreateInfo = std::move(vk::SwapchainCreateInfoKHR(vk::SwapchainCreateFlagsKHR(),
			surface,
			swapchainImageCount,
			surfaceFormat.format,
			surfaceFormat.colorSpace,
			extent,
			1,
			vk::ImageUsageFlagBits::eColorAttachment));

		swapchainCreateInfo.queueFamilyIndexCount = std::move(static_cast<uint32_t>(allowedQueueIndices.size()));
		swapchainCreateInfo.pQueueFamilyIndices = std::move(allowedQueueIndices.data());

		if (allowedQueueIndices.size() > 1)
		{
			swapchainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		}

		else
		{
			swapchainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
		}

		swapchainCreateInfo.preTransform = std::move(surfaceCapabilities.currentTransform);

		swapchainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

		swapchainCreateInfo.presentMode = std::move(presentMode);

		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = std::move(vk::SwapchainKHR(nullptr));

		try
		{
			result.value = device.createSwapchainKHR(swapchainCreateInfo);
		}
		catch (vk::SystemError err)
		{
			result.error = SwapchainError::SWAPCHAIN_CREATION_ERROR;
		}

		return result;
	}

	/*@brief (The recommended way to create a swapchain)
	* Creates a vulkan swapchain from a given vulkan device using a given vulkan physicalDevice, surface, present mode, a GLFW window and a vector of allowed queue family indices
	*
	* @param physicalDevice the vulkan physical device used to query surface capabilities
	* @param device the vulkan device from which the swapchain is created
	* @param surface the vulkan surface used to get its capabilities
	* @param pWindow the GLFW window used to query the swapchain 2D extent
	* @param presentMode the present mode to be used during swapchain creation
	* @param surfaceFormat the surface format to be used during swapchain creation
	* @param extent the 2D extent to be used during swapchain creation
	* @param allowedQueueIndices a vector of all the allowed queue indices for swapchain creation
	*
	* @return SkResult(created swapchain, SwapchainError)
	*/
	inline SkResult<vk::SwapchainKHR, SwapchainError> createSwapchain(vk::PhysicalDevice physicalDevice, vk::Device device, vk::SurfaceKHR surface, GLFWwindow* pWindow, vk::PresentModeKHR presentMode,
		vk::SurfaceFormatKHR surfaceFormat, vk::Extent2D extent, std::vector<uint32_t> allowedQueueIndices)
	{
		SkResult result(static_cast<vk::SwapchainKHR>(vk::SwapchainKHR(nullptr)), static_cast<SwapchainError>(SwapchainError::NO_ERROR));

		vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
		uint32_t swapchainImageCount = surfaceCapabilities.minImageCount + 1;
		if (surfaceCapabilities.maxImageCount > 0 && swapchainImageCount > surfaceCapabilities.maxImageCount)
		{
			swapchainImageCount = std::move(surfaceCapabilities.maxImageCount);
		}

		vk::SwapchainCreateInfoKHR swapchainCreateInfo = std::move(vk::SwapchainCreateInfoKHR(vk::SwapchainCreateFlagsKHR(),
			surface,
			swapchainImageCount,
			surfaceFormat.format,
			surfaceFormat.colorSpace,
			extent,
			1,
			vk::ImageUsageFlagBits::eColorAttachment));

		swapchainCreateInfo.queueFamilyIndexCount = std::move(static_cast<uint32_t>(allowedQueueIndices.size()));
		swapchainCreateInfo.pQueueFamilyIndices = std::move(allowedQueueIndices.data());

		if (allowedQueueIndices.size() > 1)
		{
			swapchainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		}

		else
		{
			swapchainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
		}

		swapchainCreateInfo.preTransform = std::move(surfaceCapabilities.currentTransform);
		swapchainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

		swapchainCreateInfo.presentMode = std::move(presentMode);

		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = std::move(vk::SwapchainKHR(nullptr));

		try
		{
			result.value = device.createSwapchainKHR(swapchainCreateInfo);
		}
		catch (vk::SystemError err)
		{
			result.error = SwapchainError::SWAPCHAIN_CREATION_ERROR;
		}

		return result;
	}

	/*@brief Destroys the given swapchain using the given device
	*
	* @param device the vulkan device used to destroy the swapchain
	* @param swapchain the vulkan swapchain to be destroyed
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), SwapchainError)
	*/
	inline SkResult<bool, SwapchainError> destroySwapchain(vk::Device device, vk::SwapchainKHR swapchain)
	{
		SkResult result(static_cast<bool>(true), static_cast<SwapchainError>(SwapchainError::NO_ERROR));

		try
		{
			device.destroySwapchainKHR(swapchain);
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = SwapchainError::SWAPCHAIN_DESTRUCTION_ERROR;
		}

		return result;
	}

	/*@brief Returns the swapchain images from a given swapchain by a given device
	*
	* @param device the vulkan device used to get the swapchain images
	* @param swapchain the vulkan swapchain from which the images are returned
	*
	* @return SkResult(vector of swapchain images, SwapchainError)
	*/
	inline SkResult<std::vector<vk::Image>, SwapchainError> getSwapchainImages(vk::Device device, vk::SwapchainKHR swapchain)
	{
		SkResult result(static_cast<std::vector<vk::Image>>(std::vector<vk::Image>()), static_cast<SwapchainError>(SwapchainError::NO_ERROR));

		std::vector<vk::Image> swapchainImages;
		try
		{
			swapchainImages = device.getSwapchainImagesKHR(swapchain);
		}
		catch (vk::SystemError err)
		{
			result.error = SwapchainError::GET_SWAPCHAIN_IMAGES_ERROR;
		}

		result.value = std::move(swapchainImages);
		return result;
	}

	/*@brief (Not the recommended way of creating image views, please refer to createSwapchainImageViews() using a vector of swapchainImages instead of the swapchain itself)
	* Creates swapchain image views from swapchain images using a given vulkan device and a given vulkan surface format
	*
	* @param device the vulkan device used to create these image views
	* @param swapchain the vulkan swapchain from which the original images are queried
	* @param surfaceFormat the vulkan surface format used for created image views
	*
	* @return SkResult(vector of created image views, SwapchainError)
	*/
	inline SkResult<std::vector<vk::ImageView>, SwapchainError> createSwapchainImageViews(vk::Device device, vk::SwapchainKHR swapchain, vk::SurfaceFormatKHR surfaceFormat)
	{
		SkResult result(static_cast<std::vector<vk::ImageView>>(std::vector<vk::ImageView>()), static_cast<SwapchainError>(SwapchainError::NO_ERROR));

		std::vector<vk::Image> swapchainImages = retLog(getSwapchainImages(device, swapchain));

		uint32_t swapchainImageCount = static_cast<uint32_t>(swapchainImages.size());
		std::vector<vk::ImageView> swapchainImageViews;
		swapchainImageViews.reserve(swapchainImageCount);

		for (uint32_t i = 0; i < swapchainImageCount; i++)
		{
			vk::ImageViewCreateInfo imageViewCreateInfo = {};

			imageViewCreateInfo.image = swapchainImages[i];
			imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
			imageViewCreateInfo.format = surfaceFormat.format;

			imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;

			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			try
			{
				swapchainImageViews.emplace_back(vk::ImageView(device.createImageView(imageViewCreateInfo)));
			}
			catch (vk::SystemError err)
			{
				result.error = SwapchainError::SWAPCHAIN_IMAGE_VIEW_CREATION_ERROR;
			}
		}

		result.value = std::move(swapchainImageViews);
		return result;
	}

	/*@brief Creates swapchain image views from given swapchain images using a given vulkan device and a given vulkan surface format
	*
	* @param device the vulkan device used to create these image views
	* @param swapchainImages the vulkan swapchain images from which the image views are created
	* @param surfaceFormat the vulkan surface format used for created image views
	*
	* @return SkResult(vector of created image views, SwapchainError)
	*/
	inline SkResult<std::vector<vk::ImageView>, SwapchainError> createSwapchainImageViews(vk::Device device, std::vector<vk::Image>& swapchainImages, vk::SurfaceFormatKHR& surfaceFormat)
	{
		SkResult result(static_cast<std::vector<vk::ImageView>>(std::vector<vk::ImageView>()), static_cast<SwapchainError>(SwapchainError::NO_ERROR));

		uint32_t swapchainImageCount = static_cast<uint32_t>(swapchainImages.size());
		std::vector<vk::ImageView> swapchainImageViews;
		swapchainImageViews.reserve(swapchainImageCount);

		for (uint32_t i = 0; i < swapchainImageCount; i++)
		{
			vk::ImageViewCreateInfo imageViewCreateInfo = {};

			imageViewCreateInfo.image = swapchainImages[i];
			imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
			imageViewCreateInfo.format = surfaceFormat.format;

			imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
			imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;

			imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			try
			{
				swapchainImageViews.emplace_back(device.createImageView(imageViewCreateInfo));
			}
			catch (vk::SystemError err)
			{
				result.error = SwapchainError::SWAPCHAIN_IMAGE_VIEW_CREATION_ERROR;
			}
		}

		result.value = std::move(swapchainImageViews);
		return result;
	}

	/*@brief Destroys a given vector of swapchain image views using a given device
	*
	* @param device the vulkan device used to destroy the swapchain image views
	* @param swapchainImageViews the vulkan swapchain image views to be destroyed
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), SwapchainError>
	*/
	inline SkResult<bool, SwapchainError> destroySwapchainImageViews(vk::Device device, std::vector<vk::ImageView> swapchainImageViews)
	{
		SkResult result(static_cast<bool>(true), static_cast<SwapchainError>(SwapchainError::NO_ERROR));

		for (uint32_t i = 0; i < swapchainImageViews.size(); i++)
		{
			try
			{
				device.destroyImageView(swapchainImageViews[i]);
			}
			catch (vk::SystemError err)
			{
				result.value = false;
				result.error = SwapchainError::SWAPCHAIN_IMAGE_VIEW_DESTRUCTION_ERROR;
			}
		}

		return result;
	}

	/*Command Pool*/

	/*@brief Creates a command pool using a given device from a particular queue family type (see QueueFamilyType enum)
	*
	* @param device The vulkan device used to create the command pool
	* @param queueFamilyIndexes the queue family indexes from which a particular queue family will be used in the command pool
	* @param queueFamilyType the specific type of queue family to be used in the command pool creation
	*
	* @return SkResult(created command pool, CommandPoolError)
	*/
	inline SkResult<vk::CommandPool, CommandPoolError> createCommandPool(vk::Device device, std::vector<uint32_t> queueFamilyIndexes, QueueFamilyType queueFamilyType)
	{
		SkResult result(static_cast<vk::CommandPool>(vk::CommandPool(nullptr)), static_cast<CommandPoolError>(CommandPoolError::NO_ERROR));

		vk::CommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

		if (queueFamilyIndexes[static_cast<uint32_t>(queueFamilyType)] != std::numeric_limits<uint32_t>::max())
		{
			commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndexes[static_cast<uint32_t>(queueFamilyType)];
		}
		else
		{
			result.error = CommandPoolError::WRONG_QUEUE_INDEX;
		}

		try
		{
			result.value = device.createCommandPool(commandPoolCreateInfo);
		}
		catch (vk::SystemError err)
		{
			result.error = CommandPoolError::COMMAND_POOL_CREATION_ERROR;
		}

		return result;
	}

	/*@brief Creates a vector of command pools using a given device from a given vector of queueFamilyIndexes (following the QueueFamilyType ordering, see QueueFamilyType)
	*
	* @param device The vulkan device used to create the vector of command pools
	* @param queueFamilyIndexes the queue family indexes from which the command pools will be created
	*
	* @return SkResult(vector of created command pools, CommandPoolError)
	*/
	inline SkResult<std::vector<vk::CommandPool>, CommandPoolError> createCommandPools(vk::Device device, std::vector<uint32_t> queueFamilyIndexes)
	{
		SkResult result(static_cast<std::vector<vk::CommandPool>>(std::vector<vk::CommandPool>()), static_cast<CommandPoolError>(CommandPoolError::NO_ERROR));

		std::vector<vk::CommandPool> commandPools;
		commandPools.reserve(queueFamilyIndexes.size());

		//Implying use of the QueueFamilyType ordering, see QueueFamilyType
		for (int i = 0; i < queueFamilyIndexes.size(); i++)
		{
			auto createCommandPoolResult = createCommandPool(device, queueFamilyIndexes, static_cast<QueueFamilyType>(i));
			result.error = affectError(createCommandPoolResult, result.error);
			vk::CommandPool commandPool = retLog(createCommandPoolResult);

			commandPools.emplace_back(commandPool);
		}

		result.value = commandPools;
		return result;
	}

	/*@brief Destroys the given vulkan command pool using the given vulkan device
	*
	* @param device the vulkan device used to destroy the command pool
	* @param commandPool the vulkan command pool to be destroyed
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), CommandPoolError)
	*/
	inline SkResult<bool, CommandPoolError> destroyCommandPool(vk::Device device, vk::CommandPool commandPool)
	{
		SkResult result(static_cast<bool>(true), static_cast<CommandPoolError>(CommandPoolError::NO_ERROR));

		try
		{
			device.destroyCommandPool(commandPool);
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = CommandPoolError::COMMAND_POOL_DESTRUCTION_ERROR;
		}

		return result;
	}

	/*@brief Destroys the given vulkan command pools using the given vulkan device
	*
	* @param device the vulkan device used to destroy the command pools
	* @param commandPools the vulkan command pools to be destroyed
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), CommandPoolError)
	*/
	inline SkResult<bool, CommandPoolError> destroyCommandPools(vk::Device device, std::vector<vk::CommandPool> commandPools)
	{
		SkResult result(static_cast<bool>(true), static_cast<CommandPoolError>(CommandPoolError::NO_ERROR));

		for (const auto& commandPool : commandPools)
		{
			auto destroyResult = destroyCommandPool(device, commandPool);
			result.error = affectError(destroyResult, result.error);
		}

		if (error(result))
		{
			result.value = true;
		}

		return result;
	}

	/*COMMAND BUFFER*/

	/*@brief Allocates a single vulkan command buffer given a vulkan command pool
	*
	* @param device The vulkan device allocating the command buffer
	* @param commandPool The vulkan command pool used for allocating the command buffer
	*
	* @return SkResult(vulkan command buffer, CommandBufferError)
	*/
	inline SkResult<vk::CommandBuffer, CommandBufferError> allocateCommandBuffer(vk::Device device, vk::CommandPool commandPool)
	{
		SkResult result(static_cast<vk::CommandBuffer>(vk::CommandBuffer(nullptr)), static_cast<CommandBufferError>(CommandBufferError::NO_ERROR));

		vk::CommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.commandBufferCount = 1;
		commandBufferAllocateInfo.level = vk::CommandBufferLevel::ePrimary;

		try
		{
			result.value = device.allocateCommandBuffers(commandBufferAllocateInfo)[0];
		}
		catch (vk::SystemError err)
		{
			result.error = CommandBufferError::COMMAND_BUFFER_ALLOCATION_ERROR;
		}

		return result;
	}

	/*@brief Allocates a number of vulkan command buffer given a vulkan command pool
	*
	* @param device The vulkan device allocating the command buffer
	* @param commandPool The vulkan command pool used for allocating the command buffers
	* @param commandBufferCount The number of vulkan command buffers to be allocated
	*
	* @return SkResult(vector of vulkan command buffers, CommandBufferError)
	*/
	inline SkResult<std::vector<vk::CommandBuffer>, CommandBufferError> allocateCommandBuffers(vk::Device device, vk::CommandPool commandPool, uint32_t commandBufferCount)
	{
		SkResult result(static_cast<std::vector<vk::CommandBuffer>>(std::vector<vk::CommandBuffer>()), static_cast<CommandBufferError>(CommandBufferError::NO_ERROR));

		vk::CommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.commandBufferCount = commandBufferCount;
		commandBufferAllocateInfo.level = vk::CommandBufferLevel::ePrimary;

		try
		{
			result.value = device.allocateCommandBuffers(commandBufferAllocateInfo);
		}
		catch (vk::SystemError err)
		{
			result.error = CommandBufferError::COMMAND_BUFFER_ALLOCATION_ERROR;
		}

		return result;
	}

	/*@brief "Begins" a given vulkan command buffer
	*
	* @param commandBuffer The vulkan command buffer to be "beginned"
	*
	* @return SkResult(bool, CommandBufferError)
	*/
	inline SkResult<bool, CommandBufferError> beginCommandBuffer(vk::CommandBuffer commandBuffer)
	{
		SkResult result(static_cast<bool>(true), static_cast<CommandBufferError>(CommandBufferError::NO_ERROR));

		vk::CommandBufferBeginInfo commandBufferBeginInfo = {};
		commandBufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

		try
		{
			commandBuffer.begin(commandBufferBeginInfo);
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = CommandBufferError::COMMAND_BUFFER_BEGIN_ERROR;
		}

		return result;
	}

	/*@brief "Ends" a given vulkan command buffer
	*
	* @param commandBuffer The vulkan command buffer to be "Ended"
	*
	* @return SkResult(bool, CommandBufferError)
	*/
	inline SkResult<bool, CommandBufferError> endCommandBuffer(vk::CommandBuffer commandBuffer)
	{
		SkResult result(static_cast<bool>(true), static_cast<CommandBufferError>(CommandBufferError::NO_ERROR));

		try
		{
			commandBuffer.end();
		}
		catch (vk::SystemError err)
		{
			result.value = true;
			result.error = CommandBufferError::COMMAND_BUFFER_END_ERROR;
		}

		return result;
	}

	/*RENDER PASS*/

	/*@brief Creates an attachment description from a given vulkan surfaceFormat, initial image layout (default is undefined), final image layout (default is present src)
	*
	* @param surfaceFormat the surface format used for the attachment description
	* @param initialImageLayout Initial image layout of incoming images
	* @param finalImageLayout Final image layout of outgoing images
	*
	* @return SkResult(created attachment description, RenderPassError)
	*/
	inline SkResult<vk::AttachmentDescription, RenderPassError> createAttachmentDescription(vk::SurfaceFormatKHR surfaceFormat, vk::ImageLayout initialImageLayout = vk::ImageLayout::eUndefined, vk::ImageLayout finalImageLayout = vk::ImageLayout::ePresentSrcKHR) noexcept
	{
		SkResult result(static_cast<vk::AttachmentDescription>(vk::AttachmentDescription{}), static_cast<RenderPassError>(RenderPassError::NO_ERROR));

		vk::AttachmentDescription colorAttachment = {};

		colorAttachment.format = surfaceFormat.format;
		colorAttachment.samples = vk::SampleCountFlagBits::e1;
		colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
		colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;

		colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;

		colorAttachment.initialLayout = initialImageLayout;
		colorAttachment.finalLayout = finalImageLayout;

		result.value = std::move(colorAttachment);
		return result;
	}

	/*@brief Creates an attachment reference from a given attachmentIndex (default is 0) and a given vulkan image layout (default is color attachment optimal)
	*
	* @param attachmentIndex the index corresponding to the attachment description at this index
	* @param imageLayout the image layout of the attachment reference
	*
	* @return SkResult(created attachment reference, RenderPassError)
	*/
	inline SkResult<vk::AttachmentReference, RenderPassError> createAttachmentReference(uint32_t attachmentIndex = 0, vk::ImageLayout imageLayout = vk::ImageLayout::eColorAttachmentOptimal) noexcept
	{
		SkResult result(static_cast<vk::AttachmentReference>(vk::AttachmentReference()), static_cast<RenderPassError>(RenderPassError::NO_ERROR));

		vk::AttachmentReference colorAttachmentReference = {};

		colorAttachmentReference.attachment = attachmentIndex;
		colorAttachmentReference.layout = imageLayout;

		result.value = std::move(colorAttachmentReference);
		return result;
	}

	/*@brief Creates a subpass description from given vulkan attachment references, pipeline bind point (default is graphics) and color attachment count (default is 1)
	*
	* @param colorAttachments Vector of attachment references
	* @param pipelineBindPoint a specific pipeline bind point used in the subpass description creation
	*
	* @return SkResult(created subpass description, RenderPassError)
	*/
	inline SkResult<vk::SubpassDescription, RenderPassError> createSubpassDescription(std::vector<vk::AttachmentReference>& colorAttachments, vk::PipelineBindPoint pipelineBindPoint = vk::PipelineBindPoint::eGraphics) noexcept
	{
		SkResult result(static_cast<vk::SubpassDescription>(vk::SubpassDescription()), static_cast<RenderPassError>(RenderPassError::NO_ERROR));

		vk::SubpassDescription subpassDescription = {};

		subpassDescription.pipelineBindPoint = pipelineBindPoint;
		subpassDescription.colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size());
		subpassDescription.pColorAttachments = colorAttachments.data();

		result.value = std::move(subpassDescription);
		return result;
	}

	/*@brief Creates a render pass from a device with a given array of vulkan attachment descriptions, subpass descriptions and count of attachment descriptions and subpasses
	*
	* @param device the vulkan device used to create the render pass
	* @param attachmentDescriptions Vector of vulkan attachment descriptions used in the render pass
	* @param subpassDescriptions Vector of subpass descriptions used in the render pass
	*
	* @return SkResult(created vulkan renderPass, RenderPassError)
	*/
	inline SkResult<vk::RenderPass, RenderPassError> createRenderPass(vk::Device device, std::vector<vk::AttachmentDescription>& attachmentDescriptions, std::vector<vk::SubpassDescription>& subpassDescriptions)
	{
		SkResult result(static_cast<vk::RenderPass>(vk::RenderPass(nullptr)), static_cast<RenderPassError>(RenderPassError::NO_ERROR));

		vk::RenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
		renderPassCreateInfo.pAttachments = attachmentDescriptions.data();

		renderPassCreateInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
		renderPassCreateInfo.pSubpasses = subpassDescriptions.data();

		try
		{
			result.value = device.createRenderPass(renderPassCreateInfo);
		}
		catch (vk::SystemError err)
		{
			result.error = RenderPassError::RENDER_PASS_CREATION_ERROR;
		}

		return result;
	}

	/*@brief Creates a basic vulkan render pass with default parameters
	*
	* @param device The vulkan device used to create the basic vulkan renderpass
	* @param surfaceFormat The vulkan surface format to be used in the creation of the basic vulkan render pass
	*
	* @return SkResult(created vulkan render pass, RenderPassError)
	*/
	inline SkResult<vk::RenderPass, RenderPassError> createBasicRenderPass(vk::Device device, vk::SurfaceFormatKHR surfaceFormat)
	{
		SkResult result(static_cast<vk::RenderPass>(vk::RenderPass(nullptr)), static_cast<RenderPassError>(RenderPassError::NO_ERROR));

		auto createAttachmentDescriptionResult = createAttachmentDescription(surfaceFormat);
		result.error = affectError(createAttachmentDescriptionResult, result.error);
		vk::AttachmentDescription attachmentDescription = retLog(createAttachmentDescriptionResult);
		std::vector<vk::AttachmentDescription> attachmentDescriptions = { std::move(attachmentDescription) };

		auto  createAttachmentReferenceResult = createAttachmentReference();
		result.error = affectError(createAttachmentReferenceResult, result.error);
		vk::AttachmentReference attachmentReference = retLog(createAttachmentReferenceResult);
		std::vector<vk::AttachmentReference> attachmentReferences = { std::move(attachmentReference) };

		auto createSubpassDescriptionResult = createSubpassDescription(attachmentReferences);
		result.error = affectError(createSubpassDescriptionResult, result.error);
		vk::SubpassDescription subpassDescription = retLog(createSubpassDescriptionResult);
		std::vector<vk::SubpassDescription> subpassDescriptions = { std::move(subpassDescription) };

		auto createRenderPassResult = createRenderPass(device, attachmentDescriptions, subpassDescriptions);
		result.error = affectError(createRenderPassResult, result.error);
		vk::RenderPass renderPass = retLog(createRenderPassResult);

		result.value = std::move(renderPass);
		return result;
	}
	/*@brief Destroys a given vulkan render pass from a given vulkan device
	*
	* @param device the vulkan device used to destroy the vulkan render pass
	* @param the vulkan render pass to be destroyed
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), RenderPassError)
	*/
	inline SkResult<bool, RenderPassError> destroyRenderPass(vk::Device device, vk::RenderPass renderPass)
	{
		SkResult result(static_cast<bool>(true), static_cast<RenderPassError>(RenderPassError::NO_ERROR));

		try
		{
			device.destroyRenderPass(renderPass);
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = RenderPassError::RENDER_PASS_DESTRUCTION_ERROR;
		}

		return result;
	}

	/*@brief Begins a given render pass
	*
	* @param commandBuffer the vulkan command buffer used to begin the render pass
	* @param renderPass the actual vulkan render pass beginning
	* @param extent the vulkan 2D extent to be used in the render pass
	* @param framebuffer the vulkan framebuffer to be used in the render pass
	* @param clearValue the vulkan clearValue to be used in the render pass
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), RenderPassError)
	*/
	inline SkResult<bool, RenderPassError> beginRenderPass(vk::CommandBuffer commandBuffer, vk::RenderPass renderPass, vk::Extent2D extent, vk::Framebuffer framebuffer, vk::ClearValue clearValue)
	{
		SkResult result(static_cast<bool>(true), static_cast<RenderPassError>(RenderPassError::NO_ERROR));

		vk::RenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.renderPass = renderPass;

		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent = extent;

		renderPassBeginInfo.framebuffer = framebuffer;

		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearValue;

		try
		{
			commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
		}
		catch (vk::SystemError err)
		{
			result.value = true;
			result.error = RenderPassError::RENDER_PASS_BEGIN_ERROR;
		}

		return result;
	}

	/*@brief Ends the render pass
	*
	* @param the command buffer used to end the render pass it is assigned to
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), RenderPassError)
	*/
	inline SkResult<bool, RenderPassError> endRenderPass(vk::CommandBuffer commandBuffer)
	{
		SkResult result(static_cast<bool>(true), static_cast<RenderPassError>(RenderPassError::NO_ERROR));

		try
		{
			commandBuffer.endRenderPass();
		}
		catch (vk::SystemError err)
		{
			result.value = true;
			result.error = RenderPassError::RENDER_PASS_END_ERROR;
		}

		return result;
	}

	/*FRAMEBUFFER*/

	/*@brief Creates a vector of framebuffers from a given vulkan device, render pass, 2D extent, swapchain and an attachment count and a vector of swapchain image views
	*
	* @param device the vulkan device used to create the framebuffers
	* @param renderPass the vulkan render pass to be used in the framebuffers' creation
	* @param windowExtent the vulkan 2D extent to be used in the framebuffers' creation
	* @param swapchain the vulkan swapchain to be used in the framebuffers' creation
	* @param swapchainImageViews a vector of swapchain image views the framebuffers a created from
	*
	* @return SkResult(created framebuffers, FramebufferError)
	*/
	inline SkResult<std::vector<vk::Framebuffer>, FramebufferError> createFramebuffers(vk::Device device, vk::RenderPass renderPass, uint32_t attachmentCount, vk::Extent2D windowExtent, vk::SwapchainKHR swapchain,
		std::vector<vk::ImageView> swapchainImageViews)
	{
		SkResult result(static_cast<std::vector<vk::Framebuffer>>(std::vector<vk::Framebuffer>()), static_cast<FramebufferError>(FramebufferError::NO_ERROR));

		vk::FramebufferCreateInfo framebufferCreateInfo = {};

		framebufferCreateInfo.renderPass = renderPass;
		framebufferCreateInfo.attachmentCount = attachmentCount;
		framebufferCreateInfo.width = windowExtent.width;
		framebufferCreateInfo.height = windowExtent.height;
		framebufferCreateInfo.layers = 1;

		const uint32_t swapchainImageCount = static_cast<uint32_t>(getSwapchainImages(device, swapchain).value.size());
		std::vector<vk::Framebuffer> framebuffers;
		framebuffers.reserve(swapchainImageCount);

		for (uint32_t i = 0; i < swapchainImageCount; i++)
		{
			framebufferCreateInfo.pAttachments = &swapchainImageViews[i];

			try
			{
				framebuffers.emplace_back(device.createFramebuffer(framebufferCreateInfo));
			}
			catch (vk::SystemError err)
			{
				result.error = FramebufferError::FRAMEBUFFER_CREATION_ERROR;
			}
		}

		result.value = std::move(framebuffers);
		return result;
	}

	/*@brief Destroys a given array of vulkan framebuffers from a vulkan device
	*
	* @param device the vulkan device destroyed the framebuffers
	* @param framebuffers a given vector of framebuffers to be destroyed
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), FramebufferError)
	*/
	inline SkResult<bool, FramebufferError> destroyFramebuffers(vk::Device device, std::vector<vk::Framebuffer> framebuffers)
	{
		SkResult result(static_cast<bool>(true), static_cast<FramebufferError>(FramebufferError::NO_ERROR));

		for (uint32_t i = 0; i < framebuffers.size(); i++)
		{
			try
			{
				device.destroyFramebuffer(framebuffers[i]);
			}
			catch (vk::SystemError err)
			{
				result.value = false;
				result.error = FramebufferError::FRAMEBUFFER_DESTRUCTION_ERROR;
			}
		}

		return result;
	}

	/*SYNCHRONIZATION*/

	/*@brief Creates a vulkan fence from a given vulkan device
	*
	* @param device the vulkan device creating the fence
	*
	* @return SkResult(created fence, SyncError)
	*/
	inline SkResult<vk::Fence, SyncError> createFence(vk::Device device)
	{
		SkResult result(static_cast<vk::Fence>(vk::Fence(nullptr)), static_cast<SyncError>(SyncError::NO_ERROR));

		vk::FenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled;

		try
		{
			result.value = device.createFence(fenceCreateInfo);
		}
		catch (vk::SystemError err)
		{
			result.error = SyncError::FENCE_CREATION_ERROR;
		}

		return result;
	}

	/*@brief Destroys a given vulkan fence from a vulkan device
	*
	* @param device the vulkan device used to destroy the vulkan fence
	* @param fence the vulkan fence to be destroyed
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), SyncError)
	*/
	inline SkResult<bool, SyncError> destroyFence(vk::Device device, vk::Fence fence)
	{
		SkResult result(static_cast<bool>(true), static_cast<SyncError>(SyncError::NO_ERROR));

		try
		{
			device.destroyFence(fence);
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = SyncError::FENCE_DESTRUCTION_ERROR;
		}

		return result;
	}

	/*@brief Creates a vulkan semaphore from a given vulkan device
	*
	* @param device the vulkan device creating the vulkan semaphore
	*
	* @return SkResult(created semaphore, SyncError)
	*/
	inline SkResult<vk::Semaphore, SyncError> createSemaphore(vk::Device device)
	{
		SkResult result(static_cast<vk::Semaphore>(vk::Semaphore(nullptr)), static_cast<SyncError>(SyncError::NO_ERROR));

		vk::SemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.flags = vk::SemaphoreCreateFlagBits();

		try
		{
			result.value = device.createSemaphore(semaphoreCreateInfo);
		}
		catch (vk::SystemError err)
		{
			result.error = SyncError::SEMAPHORE_CREATION_ERROR;
		}

		return result;
	}

	/*@brief Destroys a given vulkan semaphore from a vulkan device
	*
	* @param device the vulkan device used to destroy the vulkan semaphore
	* @param semaphore the vulkan semaphore to be destroyed
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), SyncError)
	*/
	inline SkResult<bool, SyncError> destroySemaphore(vk::Device device, vk::Semaphore semaphore)
	{
		SkResult result(static_cast<bool>(true), static_cast<SyncError>(SyncError::NO_ERROR));

		try
		{
			device.destroySemaphore(semaphore);
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = SyncError::SEMAPHORE_DESTRUCTION_ERROR;
		}

		return result;
	}

	/*DRAWING*/

	//Lots of work to do on that one
	/*@brief The main drawing function
	*
	* @param device the vulkan device used to acquire the next images
	* @param commandBuffer the vulkan command buffer receiving all the commands
	* @param swapchain the vulkan swapchain to get the next images from
	* @param queue the vulkan queue from which to submit
	* @param pipeline The vulkan pipeline used to draw
	* @param renderFence The vulkan fence ensuring render synchronization
	* @param presentSemaphore The vulkan semaphor ensuring present synchronization
	* @param renderSemaphore The vulkan semaphore ensuring render synchronization
	* @param renderPass The vulkan render pass to be used in the drawing process
	* @param extent The vulkan 2D extent used in the vulkan render pass
	* @param framebuffers The vulkan framebuffers to be drawn
	* @param vertexBuffers The vulkan buffers of vertices
	* @param vertices The vertices to be drawn
	* @param frameNumber The current frame number
	*
	* @return SkResult(bool signaling if the operation worked(1) or not(0), DrawingError)
	*/
	inline SkResult<bool, DrawingError> draw(vk::Device device, vk::CommandBuffer commandBuffer, vk::SwapchainKHR swapchain, vk::Queue queue, vk::Pipeline pipeline, vk::Fence renderFence, vk::Semaphore presentSemaphore,
		vk::Semaphore renderSemaphore, vk::RenderPass renderPass, vk::Extent2D extent, std::vector<vk::Framebuffer> framebuffers, std::vector<vk::Buffer> vertexBuffers,
		std::vector<Vertex> vertices, double& frameNumber)
	{
		SkResult result(static_cast<bool>(true), static_cast<DrawingError>(DrawingError::NO_ERROR));

		uint32_t timeout = 1000000000;
		auto waitForFencesResult = device.waitForFences(renderFence, true, timeout);
		device.resetFences(renderFence);

		uint32_t swapchainImageIndex = device.acquireNextImageKHR(swapchain, timeout, presentSemaphore).value;

		commandBuffer.reset();

		beginCommandBuffer(commandBuffer);

		vk::ClearValue clearValue;
		double flash = abs(sin((frameNumber) / 1200.0));
		std::array<float, 4> colorArray = { 0.0f, 0.0f, 0.0f, 1.0 };
		clearValue.color = std::move(vk::ClearColorValue(colorArray));

		beginRenderPass(commandBuffer, renderPass, extent, framebuffers[swapchainImageIndex], clearValue);

		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

		vk::DeviceSize offset = 0;
		commandBuffer.bindVertexBuffers(0, 1, vertexBuffers.data(), &offset);

		commandBuffer.draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0);

		endRenderPass(commandBuffer);

		endCommandBuffer(commandBuffer);

		queueSubmit(queue, commandBuffer, presentSemaphore, renderSemaphore, renderFence);

		queuePresent(queue, swapchain, renderSemaphore, swapchainImageIndex);

		frameNumber++;

		return result;
	}

	/*SHADERS*/

	/*@brief Loads a shader file into a vector uint32_t buffer from a given filename
	*
	* @param shaderFilename the filename referring to the shader file
	*
	* @return SkResult(actual buffer holding shader file, ShaderError)
	*/
	inline SkResult<std::vector<uint32_t>, ShaderError> loadShaderFileInBuffer(std::string shaderFilename) noexcept
	{
		SkResult result(static_cast<std::vector<uint32_t>>(std::vector<uint32_t>()), static_cast<ShaderError>(ShaderError::NO_ERROR));

		std::ifstream shaderFile(shaderFilename, std::ios::ate | std::ios::binary);

		if (!shaderFile.is_open())
		{
			result.error = ShaderError::SHADER_FILE_OPENING_ERROR;
		}

		size_t shaderFileSize = static_cast<size_t>(shaderFile.tellg());

		std::vector<uint32_t> shaderBuffer(shaderFileSize / sizeof(uint32_t));

		shaderFile.seekg(0);

		shaderFile.read((char*)shaderBuffer.data(), shaderFileSize);

		shaderFile.close();

		result.value = std::move(shaderBuffer);
		return result;
	}

	/*@brief Creates a shader module from a shaderBuffer by a vulkan device
	*
	* @param device the vulkan device creating the shader module
	* @param shaderBuffer the vector uin32_t buffer holding the shader file data
	*
	* @return SkResult(created shader module, ShaderError)
	*/
	inline SkResult<vk::ShaderModule, ShaderError> createShaderModule(vk::Device device, std::vector<uint32_t> shaderBuffer)
	{
		SkResult result(static_cast<vk::ShaderModule>(vk::ShaderModule(nullptr)), static_cast<ShaderError>(ShaderError::NO_ERROR));

		vk::ShaderModuleCreateInfo shaderModuleCreateInfo = {};
		shaderModuleCreateInfo.codeSize = shaderBuffer.size() * sizeof(uint32_t);
		shaderModuleCreateInfo.pCode = shaderBuffer.data();

		try
		{
			result.value = device.createShaderModule(shaderModuleCreateInfo);
		}
		catch (vk::SystemError err)
		{
			result.error = ShaderError::SHADER_MODULE_CREATION_ERROR;
		}

		return result;
	}

	/*@brief Creates a shader module from a shader filename by a vulkan device
	*
	* @param device the vulkan device creating the shader module
	* @param shaderFilename std::string containing the shader filename
	*
	* @return SkResult(created shader module, ShaderError)
	*/
	inline SkResult<vk::ShaderModule, ShaderError> createShaderModule(vk::Device device, std::string shaderFilename)
	{
		SkResult result(static_cast<vk::ShaderModule>(vk::ShaderModule(nullptr)), static_cast<ShaderError>(ShaderError::NO_ERROR));

		auto loadShaderFileInBufferResult = loadShaderFileInBuffer(shaderFilename);
		result.error = affectError(loadShaderFileInBufferResult, result.error);
		std::vector<uint32_t> shaderBuffer = retLog(loadShaderFileInBufferResult);

		auto createShaderModuleResult = createShaderModule(device, shaderBuffer);
		result.error = affectError(createShaderModuleResult, result.error);
		vk::ShaderModule shaderModule = retLog(createShaderModuleResult);

		result.value = shaderModule;
		return result;
	}

	/*@brief Destroys a given vulkan shader module
	*
	* @param device The vulkan device ordered to destroy the shader module
	* @param shaderModule The vulkan shader module to be destroyed
	*
	* @return SkResult(bool indicating if the operation worked(true) or not(false), ShaderError)
	*/
	inline SkResult<bool, ShaderError> destroyShaderModule(vk::Device device, vk::ShaderModule shaderModule)
	{
		SkResult result(static_cast<bool>(true), static_cast<ShaderError>(ShaderError::NO_ERROR));

		try
		{
			device.destroyShaderModule(shaderModule);
		}
		catch (vk::SystemError err)
		{
			result.value = 0;
			result.error = ShaderError::SHADER_MODULE_DELETION_ERROR;
		}

		return result;
	}

	/*PIPELINES*/

	/*@brief Creates a shader stage given its flags, module and entry name
	*
	* @param shaderStageFlag The vulkan shader stage flag indicating if the associated shader module is a vertex shader or a fragment shader
	* @param shaderModule The vulkan shader module to be included in the shader stage
	* @param entryName The name of the main() function of the shader
	*
	* @return SkResult(created shader stage, GraphicsPipelineError)
	*/
	inline SkResult<vk::PipelineShaderStageCreateInfo, GraphicsPipelineError> createPipelineShaderStageCreateInfo(vk::ShaderStageFlagBits& shaderStageFlag, vk::ShaderModule& shaderModule,
		std::string& entryName)
	{
		SkResult result(static_cast<vk::PipelineShaderStageCreateInfo>(vk::PipelineShaderStageCreateInfo{}), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
		try
		{
			pipelineShaderStageCreateInfo.stage = shaderStageFlag;
			pipelineShaderStageCreateInfo.module = shaderModule;
			pipelineShaderStageCreateInfo.pName = entryName.c_str();
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::PIPELINE_SHADER_STAGE_CREATE_INFO_ERROR;
		}

		result.value = pipelineShaderStageCreateInfo;
		return result;
	}

	/*@brief Creates a vector of shader stages given its flags, modules and entry names
	*
	* @param shaderStageFlags The vulkan shader stage flags indicating if each of the associated shaders are vertex or fragment shader
	* @param shaderModules The vulkan shader modules to be included in the shader stage, originating from a given shader
	* @param entryNames The names of the main() functions of the shaders
	*
	* @return SkResult(created shader stage, GraphicsPipelineError)
	*/
	inline SkResult<std::vector<vk::PipelineShaderStageCreateInfo>, GraphicsPipelineError> createPipelineShaderStageCreateInfos(std::vector<vk::ShaderStageFlagBits>& shaderStageFlags,
		std::vector<vk::ShaderModule>& shaderModules, std::vector<std::string>& entryNames)
	{
		SkResult result(static_cast<std::vector<vk::PipelineShaderStageCreateInfo>>(std::vector<vk::PipelineShaderStageCreateInfo>()), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
		shaderStages.reserve(shaderModules.size());
		for (int i = 0; i < shaderModules.size(); i++)
		{
			auto createShaderStageResult = createPipelineShaderStageCreateInfo(shaderStageFlags[i], shaderModules[i], entryNames[i]);
			shaderStages.emplace_back(retLog(createShaderStageResult));
		}

		result.value = std::move(shaderStages);
		return result;
	}

	/*@brief Creates a vector of shader stages given a vulkan device, a vector of shader filenames, a vector of shader stage flags and a vector of shader entry names
	*
	* @param device The vulkan device used to create the shader stages(creation of the shader modules)
	* @param shaderFilenames The shader filenames to be converted into shader modules
	* @param shaderStageFlags The shader stage flags indicating if the given shaders are either vertex or fragment shaders
	* @param entryNames
	*
	* @return SkResult(created vector of shader stages, GraphicsPipelineError)
	*/
	inline SkResult<std::vector<vk::PipelineShaderStageCreateInfo>, GraphicsPipelineError> createShaderStages(vk::Device device, std::vector<std::string>& shaderFilenames,
		std::vector<vk::ShaderStageFlagBits>& shaderStageFlags, std::vector<std::string>& entryNames)
	{
		SkResult result(static_cast<std::vector<vk::PipelineShaderStageCreateInfo>>(std::vector<vk::PipelineShaderStageCreateInfo>()), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		std::vector<vk::ShaderModule> shaderModules;
		shaderModules.reserve(shaderFilenames.size());

		for (uint32_t i = 0; i < shaderFilenames.size(); i++)
		{
			auto shaderBufferResult = loadShaderFileInBuffer(shaderFilenames[i]);
			std::vector<uint32_t> shaderBuffer = retLog(shaderBufferResult);

			auto shaderModuleResult = createShaderModule(device, shaderBuffer);
			vk::ShaderModule shaderModule = retLog(shaderModuleResult);

			shaderModules.emplace_back(std::move(shaderModule));
		}

		auto createPipelineShaderStageCreateInfosResult = createPipelineShaderStageCreateInfos(shaderStageFlags, shaderModules, entryNames);
		result.error = affectError(createPipelineShaderStageCreateInfosResult, result.error);
		std::vector<vk::PipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos = retLog(createPipelineShaderStageCreateInfosResult);

		result.value = std::move(pipelineShaderStageCreateInfos);
		return result;
	}

	/*@brief Creates a vertex input state containing given input bindings and given input attributes
	*
	* @param vertexInputBindingDescriptions vector of input binding descriptions
	* @param vertexInputAttributeDescriptions vector of input attribute descriptions
	*
	* @return SkResult(created vertex input state, GraphicsPipelineError)
	*/
	inline SkResult<vk::PipelineVertexInputStateCreateInfo, GraphicsPipelineError> createPipelineVertexInputStateCreateInfo(std::vector<vk::VertexInputBindingDescription>& vertexInputBindingDescriptions,
		std::vector<vk::VertexInputAttributeDescription>& vertexInputAttributeDescriptions)
	{
		SkResult result(static_cast<vk::PipelineVertexInputStateCreateInfo>(vk::PipelineVertexInputStateCreateInfo{}), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));;

		vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {};
		try
		{
			pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindingDescriptions.size());
			pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vertexInputBindingDescriptions.data();

			pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributeDescriptions.size());
			pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data();
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO_ERROR;
		}

		result.value = std::move(pipelineVertexInputStateCreateInfo);
		return result;
	}

	/*@brief Creates an input assembly state containing a given primitive topology
	*
	* @param primitiveTopology the primitive topology (point, line list/strip, triangle list/strip) to be used in the pipeline
	*
	* @return SkResult(created assembly state, GraphicsPipelineError)
	*/
	inline SkResult<vk::PipelineInputAssemblyStateCreateInfo, GraphicsPipelineError> createPipelineInputAssemblyStateCreateInfo(vk::PrimitiveTopology& primitiveTopology)
	{
		SkResult result(static_cast<vk::PipelineInputAssemblyStateCreateInfo>(vk::PipelineInputAssemblyStateCreateInfo{}), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {};
		try
		{
			pipelineInputAssemblyStateCreateInfo.topology = primitiveTopology;
			pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO_ERROR;
		}

		result.value = std::move(pipelineInputAssemblyStateCreateInfo);
		return result;
	}

	/*@brief Creates a rasterization state containg a given polygonMode
	*
	* @param polygonMode The polygon mode to be used (fill, line, point) in the pipeline
	*
	* @return SkResult(created rasterization state, GraphicsPipelineError)
	*/
	inline SkResult<vk::PipelineRasterizationStateCreateInfo, GraphicsPipelineError> createPipelineRasterizationStateCreateInfo(vk::PolygonMode& polygonMode)
	{
		SkResult result(static_cast<vk::PipelineRasterizationStateCreateInfo>(vk::PipelineRasterizationStateCreateInfo{}), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = {};
		try
		{
			pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
			pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;

			pipelineRasterizationStateCreateInfo.polygonMode = polygonMode;
			pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;

			pipelineRasterizationStateCreateInfo.cullMode = vk::CullModeFlagBits::eNone;
			pipelineRasterizationStateCreateInfo.frontFace = vk::FrontFace::eClockwise;

			pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
			pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
			pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
			pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::PIPELINE_RASTERIZATION_STATE_CREATE_INFO_ERROR;
		}

		result.value = std::move(pipelineRasterizationStateCreateInfo);
		return result;
	}

	/*@brief Creates a multisample state
	*
	* @param sampleShadingEnabled Is sample shading enabled ?
	* @param sampleCountFlag Flags to be used if sample shading is enabled
	*
	* @return SkResult(created multisample state, GraphicsPipelineError)
	*/
	inline SkResult<vk::PipelineMultisampleStateCreateInfo, GraphicsPipelineError> createPipelineMultisampleStateCreateInfo(bool sampleShadingEnabled = false,
		vk::SampleCountFlagBits sampleCountFlag = vk::SampleCountFlagBits::e1)
	{
		SkResult result(static_cast<vk::PipelineMultisampleStateCreateInfo>(vk::PipelineMultisampleStateCreateInfo{}), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo = {};
		try
		{
			if (sampleShadingEnabled)
			{
				pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_TRUE;
				pipelineMultisampleStateCreateInfo.rasterizationSamples = sampleCountFlag;

			}
			else
			{
				pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
				pipelineMultisampleStateCreateInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;
			}

			pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f;
			pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;

			pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
			pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::PIPELINE_MULTISAMPLE_STATE_CREATE_INFO_ERROR;
		}

		result.value = std::move(pipelineMultisampleStateCreateInfo);
		return result;
	}

	/*@brief Creates a viewport according to a given extent
	*
	* @param extent The vulkan 2D extent to be used in the creation of the viewport
	*
	* @return SkResult(created viewport, GraphicsPipelineError)
	*/
	inline SkResult<vk::Viewport, GraphicsPipelineError> createViewport(vk::Extent2D& extent)
	{
		SkResult result(static_cast<vk::Viewport>(vk::Viewport{}), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		vk::Viewport viewport = {};
		try
		{
			viewport.x = 0.0f;
			viewport.y = 0.0f;

			viewport.width = static_cast<float>(extent.width);
			viewport.height = static_cast<float>(extent.height);

			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::VIEWPORT_CREATION_ERROR;
		}

		result.value = std::move(viewport);
		return result;
	}

	/*@brief Creates a scissor according to a given extent
	*
	* @param extent The vulkan 2D extent to be used to create the scissor
	*
	* @return SkResult(created scissor, GraphicsPipelineError)
	*/
	inline SkResult<vk::Rect2D, GraphicsPipelineError> createScissor(vk::Extent2D& extent)
	{
		SkResult result(static_cast<vk::Rect2D>(vk::Rect2D{}), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));


		vk::Rect2D scissor = {};
		try
		{
			scissor.offset = vk::Offset2D{ 0, 0 };
			scissor.extent = extent;
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::SCISSOR_CREATION_ERROR;
		}

		result.value = std::move(scissor);
		return result;
	}

	/*@brief Creates a viewport state containing given viewports and given scissors
	*
	* @param viewports The vector of vulkan viewports to be used in the pipeline
	* @param scissors The vector of vulkan scissors to be used in the pipeline
	*
	* @return SkResult(created viewport state, GraphicsPipelineError)
	*/
	inline SkResult<vk::PipelineViewportStateCreateInfo, GraphicsPipelineError> createPipelineViewportStateCreateInfo(std::vector<vk::Viewport>& viewports, std::vector<vk::Rect2D>& scissors)
	{
		vk::PipelineViewportStateCreateInfo value = vk::PipelineViewportStateCreateInfo{};
		GraphicsPipelineError error = GraphicsPipelineError::NO_ERROR;

		vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {};
		pipelineViewportStateCreateInfo.viewportCount = static_cast<uint32_t>(viewports.size());
		pipelineViewportStateCreateInfo.pViewports = viewports.data();

		pipelineViewportStateCreateInfo.scissorCount = static_cast<uint32_t>(scissors.size());
		pipelineViewportStateCreateInfo.pScissors = scissors.data();

		value = pipelineViewportStateCreateInfo;
		return SkResult(value, error); // Error seems to be after returning this, value is corrupted
	}

	/*@brief Creates a color blend attachment state
	*
	* @param colorWriteMask The color write mask to be used in the pipeline
	* @param blendEnabled Is blend enabled ?
	*
	* @return SkResult(created color blend attachment state, GraphicsPipelineError)
	*/
	inline SkResult<vk::PipelineColorBlendAttachmentState, GraphicsPipelineError> createPipelineColorBlendAttachmentState(vk::Flags<vk::ColorComponentFlagBits> colorWriteMask = (vk::ColorComponentFlagBits::eR |
		vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eA),
		bool blendEnabled = false)
	{
		SkResult result(static_cast<vk::PipelineColorBlendAttachmentState>(vk::PipelineColorBlendAttachmentState{}), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = {};
		try
		{
			pipelineColorBlendAttachmentState.colorWriteMask = colorWriteMask;
			pipelineColorBlendAttachmentState.blendEnable = blendEnabled ? VK_TRUE : VK_FALSE;
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::PIPELINE_COLOR_BLEND_ATTACHMENT_STATE_ERROR;
		}

		result.value = std::move(pipelineColorBlendAttachmentState);
		return result;
	}

	/*@brief Creates a color blend state given color blend attachments
	*
	* @param colorBlendAttachments The vector of color blend attachments to be used in the pipeline
	*
	* @return SkResult(created color blend state, GraphicsPipelineError)
	*/
	inline SkResult<vk::PipelineColorBlendStateCreateInfo, GraphicsPipelineError> createPipelineColorBlendStateCreateInfo(std::vector<vk::PipelineColorBlendAttachmentState>& colorBlendAttachments)
	{
		SkResult result(static_cast<vk::PipelineColorBlendStateCreateInfo>(vk::PipelineColorBlendStateCreateInfo{}), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo{};
		try
		{
			pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
			pipelineColorBlendStateCreateInfo.logicOp = vk::LogicOp::eCopy;

			pipelineColorBlendStateCreateInfo.attachmentCount = static_cast<uint32_t>(colorBlendAttachments.size());
			pipelineColorBlendStateCreateInfo.pAttachments = colorBlendAttachments.data();
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::PIPELINE_COLOR_BLEND_STATE_CREATE_INFO_ERROR;
		}

		result.value = std::move(pipelineColorBlendStateCreateInfo);
		return result;
	}

	/*@brief Creates a pipeline layout with a given device
	*
	* @param device The vulkan device used to create the pipeline layout
	*
	* @return SkResult(created pipeline layout, GraphicsPipelineError)
	*/
	inline SkResult<vk::PipelineLayout, GraphicsPipelineError> createPipelineLayout(vk::Device device)
	{
		SkResult result(static_cast<vk::PipelineLayout>(vk::PipelineLayout(nullptr)), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
		try
		{
			pipelineLayoutCreateInfo.setLayoutCount = 0;
			pipelineLayoutCreateInfo.pSetLayouts = nullptr;

			pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
			pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::PIPELINE_LAYOUT_CREATE_INFO_ERROR;
		}

		vk::PipelineLayout pipelineLayout = vk::PipelineLayout(nullptr);
		try
		{
			pipelineLayout = device.createPipelineLayout(pipelineLayoutCreateInfo);
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::PIPELINE_LAYOUT_CREATION_ERROR;
		}

		result.value = pipelineLayout;
		return result;
	}

	/*@brief Destroys a given pipeline layout with a given device
	*
	* @param device The vulkan device used to destroy the given pipeline layout
	* @param pipelineLayout The vulkan pipeline layout to be destroyed
	*
	* @return SkResult(boolean indicating if the operation worked(true) or not(false), GraphicsPipelineError)
	*/
	inline SkResult<bool, GraphicsPipelineError> destroyPipelineLayout(vk::Device device, vk::PipelineLayout pipelineLayout)
	{
		SkResult result(static_cast<bool>(true), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		try
		{
			device.destroyPipelineLayout(pipelineLayout);
		}
		catch (vk::SystemError error)
		{
			result.value = false;
			result.error = GraphicsPipelineError::PIPELINE_LAYOUT_DESTRUCTION_ERROR;
		}

		return result;
	}

	/*@brief Creates a basic graphics pipeline according to the given parameters
	*
	* @param device The vulkan device ordered to create the graphics pipeline
	* @param renderPass The vulkan render pass to be used in the graphics pipeline
	* @param pipelineLayout The vulkan pipeline layout to be used in the graphics pipeline
	* @param extent The vulkan extent to be used in the graphics pipeline
	* @param shaderStageFlags The vulkan shader stage flags to be used in the graphics pipeline
	* @param shaderModules The vulkan shader modules to be used in the graphics pipeline
	* @param entryNames The vulkan entry names to be used for the shaders in the graphics pipeline
	* @param vertexInputBindingDescriptions The vulkan vertex input binding descriptions to be used in the graphics pipeline
	* @param vertexInputAttributeDescriptions The vulkan vertex input attribute descriptions to be used in the graphics pipeline
	* @param primitiveTopology The vulkan primitive topology to be used in the graphics pipeline
	* @param polygonMode The vulkan polygon mode to be used in the graphics pipeline
	* @param sampleShadingEnabled Is sample shading enabled in the graphics pipeline
	* @param sampleCountFlag The sample count to be used in the graphics pipeline
	* @param colorWriteMask The vulkan color write mask to be used in the graphics pipeline
	* @param blendEnabled Is blend enabled in the graphics pipeline
	*
	* @return SkResult(created graphics pipeline, GraphicsPipelineError)
	*/
	inline SkResult<vk::Pipeline, GraphicsPipelineError> createGraphicsPipeline(vk::Device device, vk::RenderPass renderPass, vk::PipelineLayout pipelineLayout, vk::Extent2D extent,
		std::vector<vk::ShaderStageFlagBits> shaderStageFlags, std::vector<vk::ShaderModule> shaderModules, std::vector<std::string>& entryNames,
		std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions = std::vector<vk::VertexInputBindingDescription>(),
		std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions = std::vector<vk::VertexInputAttributeDescription>(),
		vk::PrimitiveTopology primitiveTopology = vk::PrimitiveTopology::eTriangleList, vk::PolygonMode polygonMode = vk::PolygonMode::eFill,
		bool sampleShadingEnabled = false, vk::SampleCountFlagBits sampleCountFlag = vk::SampleCountFlagBits::e1,
		vk::Flags<vk::ColorComponentFlagBits> colorWriteMask = (vk::ColorComponentFlagBits::eR |
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eA),
		bool blendEnabled = false)
	{
		SkResult result(static_cast<vk::Pipeline>(vk::Pipeline(nullptr)), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		//Shader stages
		auto createPipelineShaderStageCreateInfosResult = createPipelineShaderStageCreateInfos(shaderStageFlags, shaderModules, entryNames);
		result.error = affectError(createPipelineShaderStageCreateInfosResult, result.error);
		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = retLog(createPipelineShaderStageCreateInfosResult);

		//Vertex input
		auto createPipelineVertexInputStateCreateInfoResult = createPipelineVertexInputStateCreateInfo(vertexInputBindingDescriptions, vertexInputAttributeDescriptions);
		result.error = affectError(createPipelineVertexInputStateCreateInfoResult, result.error);
		vk::PipelineVertexInputStateCreateInfo vertexInputState = retLog(createPipelineVertexInputStateCreateInfoResult);

		//Input assembly
		auto createPipelineInputAssemblyStateCreateInfoResult = createPipelineInputAssemblyStateCreateInfo(primitiveTopology);
		result.error = affectError(createPipelineInputAssemblyStateCreateInfoResult, result.error);
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState = retLog(createPipelineInputAssemblyStateCreateInfoResult);

		//Viewport
		auto createViewportResult = createViewport(extent);
		result.error = affectError(createViewportResult, result.error);
		vk::Viewport viewport = retLog(createViewportResult);

		auto createScissorResult = createScissor(extent);
		result.error = affectError(createScissorResult, result.error);
		vk::Rect2D scissor = retLog(createScissorResult);

		std::vector<vk::Viewport> viewports = { viewport };
		std::vector<vk::Rect2D> scissors = { scissor };
		auto createPipelineViewportStateCreateInfoResult = createPipelineViewportStateCreateInfo(viewports, scissors);
		result.error = affectError(createPipelineViewportStateCreateInfoResult, result.error);
		vk::PipelineViewportStateCreateInfo viewportState = retLog(createPipelineViewportStateCreateInfoResult);

		//Rasterization
		auto createPipelineRasterizationStateCreateInfoResult = createPipelineRasterizationStateCreateInfo(polygonMode);
		result.error = affectError(createPipelineRasterizationStateCreateInfoResult, result.error);
		vk::PipelineRasterizationStateCreateInfo rasterizationState = retLog(createPipelineRasterizationStateCreateInfoResult);

		//Multisample
		auto createPipelineMultisampleStateCreateInfoResult = createPipelineMultisampleStateCreateInfo();
		result.error = affectError(createPipelineMultisampleStateCreateInfoResult, result.error);
		vk::PipelineMultisampleStateCreateInfo multisampleState = retLog(createPipelineMultisampleStateCreateInfoResult);

		//Color blend
		auto createPipelineColorBlendAttachmentStateResult = createPipelineColorBlendAttachmentState();
		result.error = affectError(createPipelineColorBlendAttachmentStateResult, result.error);
		vk::PipelineColorBlendAttachmentState colorBlendAttachmentState = retLog(createPipelineColorBlendAttachmentStateResult);

		std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachmentStates = { colorBlendAttachmentState };
		auto createPipelineColorBlendStateCreateInfoResult = createPipelineColorBlendStateCreateInfo(colorBlendAttachmentStates);
		result.error = affectError(createPipelineColorBlendStateCreateInfoResult, result.error);
		vk::PipelineColorBlendStateCreateInfo colorBlendState = retLog(createPipelineColorBlendStateCreateInfoResult);

		vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
		try
		{
			graphicsPipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
			graphicsPipelineCreateInfo.pStages = shaderStages.data();

			graphicsPipelineCreateInfo.pVertexInputState = &vertexInputState;

			graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;

			graphicsPipelineCreateInfo.pViewportState = &viewportState;

			graphicsPipelineCreateInfo.pRasterizationState = &rasterizationState;

			graphicsPipelineCreateInfo.pMultisampleState = &multisampleState;

			graphicsPipelineCreateInfo.pColorBlendState = &colorBlendState;

			graphicsPipelineCreateInfo.layout = pipelineLayout;

			graphicsPipelineCreateInfo.renderPass = renderPass;

			graphicsPipelineCreateInfo.subpass = 0;

			graphicsPipelineCreateInfo.basePipelineHandle = vk::Pipeline(nullptr);
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::GRAPHICS_PIPELINE_CREATE_INFO_ERROR;
		}

		vk::Pipeline graphicsPipeline = vk::Pipeline(nullptr);

		try
		{
			auto createGraphicsPipelineResult = device.createGraphicsPipeline(vk::PipelineCache(nullptr), graphicsPipelineCreateInfo);
			if (createGraphicsPipelineResult.result != vk::Result::eSuccess)
			{
				result.error = GraphicsPipelineError::PIPELINE_CREATION_ERROR;
			}

			graphicsPipeline = createGraphicsPipelineResult.value;
		}
		catch (vk::SystemError err)
		{
			result.error = GraphicsPipelineError::PIPELINE_CREATION_ERROR;
		}

		result.value = std::move(graphicsPipeline);
		return result;
	}

	/*@brief Destroys the given pipeline using a given device
	*
	* @param device The vulkan device ordered to destroy the pipeline
	* @param pipeline The vulkan pipeline to be destroyed
	*
	* @return SkResult(boolean indicating if the operation worked(true) or not(false), GraphicsPipelineError)
	*/
	inline SkResult<bool, GraphicsPipelineError> destroyPipeline(vk::Device device, vk::Pipeline pipeline)
	{
		SkResult result(static_cast<bool>(true), static_cast<GraphicsPipelineError>(GraphicsPipelineError::NO_ERROR));

		try
		{
			device.destroyPipeline(pipeline);
		}
		catch (vk::SystemError err)
		{
			result.value = false;
			result.error = GraphicsPipelineError::PIPELINE_DELETION_ERROR;
		}

		return result;
	}

	/*Allocation*/

	/*@brief Returns the index of the chosen memory type according to the given type filter and memory properties
	*
	* @param physicalDevice The vulkan physical device to query memory properties from
	* @param typeFilter The type filter indicating which memory type should be used
	* @param memoryProperties The memory properties to be compared against physical device queried memory properties
	*
	* @return SkResult(index of memory type, AllocationError)
	*/
	inline SkResult<uint32_t, AllocationError> getMemoryTypeIndex(vk::PhysicalDevice physicalDevice, uint32_t typeFilter, vk::Flags<vk::MemoryPropertyFlagBits> memoryProperties)
	{
		SkResult result(static_cast<uint32_t>(std::numeric_limits<uint32_t>::max()), static_cast<AllocationError>(AllocationError::NO_ERROR));

		vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties = physicalDevice.getMemoryProperties();

		uint32_t memoryTypeIndex = std::numeric_limits<uint32_t>::max();

		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && ((physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & memoryProperties) == memoryProperties))
			{
				memoryTypeIndex = i;
				break;
			}
		}

		if (memoryTypeIndex == std::numeric_limits<uint32_t>::max())
		{
			result.error = AllocationError::COULD_NOT_GET_MEMORY_TYPE_INDEX_ERROR;
		}

		result.value = std::move(memoryTypeIndex);
		return result;
	}

	/*@brief Returns the aligned offset according to the given alignement and current offset
	*
	* @param alignement The alignement to follow
	* @param currentOffset The current offset before aligned
	*
	* @return SkResult(aligned offset, AllocationError)
	*/
	inline SkResult<uint64_t, AllocationError> getAlignedOffset(vk::DeviceSize alignement, uint64_t currentOffset)
	{
		SkResult result(static_cast<uint64_t>(std::numeric_limits<uint64_t>::max()), static_cast<AllocationError>(AllocationError::NO_ERROR));

		while ((currentOffset % alignement) != 0)
		{
			currentOffset++;
		}

		result.value = currentOffset;
		return result;
	}

	/*A very basic/barebone memory block, contains the id, origin and size of itself.*/
	class MemoryBlock
	{
	public:
		MemoryBlock(uint32_t identification, uint64_t offset, MemoryBlockSize size)
		{
			id = identification;
			origin = offset;
			blockSize = size;
		}

		MemoryBlock(MemoryBlockSize size)
		{
			id = 0;
			origin = 0;
			blockSize = size;
		}

		MemoryBlock()
		{
			id = 0;
			origin = 0;
			blockSize = MemoryBlockSize::ZERO;
		}

		/*Returns a pointer to the memory block*/
		inline SkResult<MemoryBlock*, AllocationError> get()
		{
			SkResult result(static_cast<MemoryBlock*>(nullptr), static_cast<AllocationError>(AllocationError::NO_ERROR));

			result.value = this;
			return result;
		}

		/*Returns the id of the memory block*/
		inline SkResult<uint32_t, AllocationError> getId()
		{
			SkResult result(static_cast<uint32_t>(std::numeric_limits<uint32_t>::max()), static_cast<AllocationError>(AllocationError::NO_ERROR));

			result.value = this->id;
			return result;
		}

		/*Returns the origin of the memory block*/
		inline SkResult<uint64_t, AllocationError> getOrigin()
		{
			SkResult result(static_cast<uint64_t>(std::numeric_limits<uint64_t>::max()), static_cast<AllocationError>(AllocationError::NO_ERROR));


			result.value = this->origin;
			return result;
		}

		/*Returns the size of to the memory block*/
		inline SkResult<MemoryBlockSize, AllocationError> getSize()
		{
			SkResult result(static_cast<MemoryBlockSize>(MemoryBlockSize::ZERO), static_cast<AllocationError>(AllocationError::NO_ERROR));

			result.value = this->blockSize;
			return result;
		}
	private:
		uint32_t id;
		uint64_t origin;

		MemoryBlockSize blockSize;

	};

	/*A very basic/barebone memory pool, holds onto device memory and a vector of blocks*/
	class MemoryPool
	{
	public:
		MemoryPool()
		{

		}
		MemoryPool(vk::Device poolDevice, MemoryPoolSize poolSize, uint32_t poolId, vk::DeviceMemory poolMemory, vk::MemoryType poolMemoryType, uint32_t poolMemoryTypeIndex)
			: device(poolDevice),
			memorySize(poolSize),
			id(poolId),
			memory(poolMemory),
			memoryType(poolMemoryType),
			memoryTypeIndex(poolMemoryTypeIndex)
		{
		}

		//Create block 
		inline SkResult<uint32_t, AllocationError> createMemoryBlock(MemoryBlockSize blockSize, vk::Buffer buffer)
		{
			SkResult result(static_cast<uint32_t>(1), static_cast<AllocationError>(AllocationError::NO_ERROR));

			vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements(buffer);

			uint32_t id = std::numeric_limits<uint32_t>::max();
			uint64_t offset = std::numeric_limits<uint32_t>::max();
			if (memoryBlocks.size() == 0)
			{
				id = 0;
				offset = 0;
			}
			else
			{
				uint32_t blockId = retLog(memoryBlocks[memoryBlocks.size() - 1]->getId());
				id = blockId + 1;

				uint64_t totalBlockSize = 0;
				for (uint32_t i = 0; i < memoryBlocks.size(); i++)
				{
					MemoryBlockSize blockSize = retLog(memoryBlocks[i]->getSize());
					totalBlockSize += static_cast<uint64_t>(blockSize);
				}

				uint64_t currentOffset = totalBlockSize + (memoryBlocks.size() * gapSize);
				auto getAlignedOffsetResult = getAlignedOffset(memoryRequirements.alignment, currentOffset);
				uint64_t alignedOffset = retLog(getAlignedOffsetResult);

				offset = alignedOffset;
			}

			MemoryBlock* block = new MemoryBlock(id, offset, blockSize);
			memoryBlocks.emplace_back(block);

			result.value = std::move(id);
			return result;
		}

		inline SkResult<bool, AllocationError> bind(MemoryBlock* block, vk::Buffer buffer)
		{
			SkResult result(static_cast<bool>(true), static_cast<AllocationError>(AllocationError::NO_ERROR));

			uint64_t offset = static_cast<uint64_t>(retLog(block->getId()));
			device.bindBufferMemory(buffer, this->memory, offset);


			return result;
		}

		inline SkResult<void*, AllocationError> map(MemoryBlock* block)
		{
			SkResult result(static_cast<void*>(nullptr), static_cast<AllocationError>(AllocationError::NO_ERROR));

			uint64_t offset = static_cast<uint64_t>(retLog(block->getId()));
			uint64_t size = static_cast<uint64_t>(retLog(block->getSize()));
			void* data = device.mapMemory(this->memory, offset, size);

			result.value = data;
			return result;
		}

		inline SkResult<bool, AllocationError> unmap()
		{
			SkResult result(static_cast<bool>(true), static_cast<AllocationError>(AllocationError::NO_ERROR));

			device.unmapMemory(this->memory);

			return result;
		}

		/*Returns the free memory of the memory pool*/
		inline SkResult<uint64_t, AllocationError> getFreeMemory()
		{
			SkResult result(static_cast<uint64_t>(0), static_cast<AllocationError>(AllocationError::NO_ERROR));

			uint64_t freePoolMemory = static_cast<uint64_t>(memorySize) - occupiedMemory;

			result.value = std::move(freePoolMemory);
			return result;
		}

		inline SkResult<uint32_t, AllocationError> getMemoryTypeIndex()
		{
			SkResult result(static_cast<uint32_t>(std::numeric_limits<uint32_t>::max()), static_cast<AllocationError>(AllocationError::NO_ERROR));

			result.value = memoryTypeIndex;
			return result;
		}

		inline SkResult<vk::MemoryType, AllocationError> getMemoryType()
		{
			SkResult result(static_cast<vk::MemoryType>(vk::MemoryType{}), static_cast<AllocationError>(AllocationError::NO_ERROR));

			result.value = memoryType;
			return result;
		}

		inline SkResult<uint32_t, AllocationError> getId()
		{
			SkResult result(static_cast<uint32_t>(std::numeric_limits<uint32_t>::max()), static_cast<AllocationError>(AllocationError::NO_ERROR));

			result.value = id;
			return result;
		}

		inline SkResult<MemoryBlock*, AllocationError> getBlockById(uint32_t blockId)
		{
			SkResult result(static_cast<MemoryBlock*>(nullptr), static_cast<AllocationError>(AllocationError::NO_ERROR));

			bool found = false;
			MemoryBlock* pMemoryBlock = nullptr;
			for (uint32_t i = 0; i < memoryBlocks.size(); i++)
			{
				uint32_t currentBlockId = retLog(memoryBlocks[i]->getId());
				if (currentBlockId == blockId)
				{
					pMemoryBlock = memoryBlocks[i];
					found = true;
				}
			}

			if (!found)
			{
				result.error = AllocationError::UNDEFINED_ERROR;
			}

			result.value = std::move(pMemoryBlock);
			return result;
		}

		inline SkResult<uint32_t, AllocationError> getNumberOfBlocks()
		{
			SkResult result(static_cast<uint32_t>(std::numeric_limits<uint32_t>::max()), static_cast<AllocationError>(AllocationError::NO_ERROR));

			result.value = static_cast<uint32_t>(memoryBlocks.size());
			return result;
		}

		inline SkResult<bool, AllocationError> freeMemoryBlocks()
		{
			SkResult result(static_cast<bool>(true), static_cast<AllocationError>(AllocationError::NO_ERROR));

			for (uint32_t i = 0; i < memoryBlocks.size(); i++)
			{
				delete memoryBlocks[i];
			}

			return result;
		}

		inline SkResult<bool, AllocationError> freeMemory()
		{
			SkResult result(static_cast<bool>(true), static_cast<AllocationError>(AllocationError::NO_ERROR));

			try
			{
				device.freeMemory(memory);
			}
			catch (vk::SystemError error)
			{
				result.value = false;
				result.error = AllocationError::UNDEFINED_ERROR;
			}

			return result;
		}

	private:
		vk::Device device;

		uint32_t id;

		std::vector<MemoryBlock*> memoryBlocks = {};
		uint64_t gapSize = 1;

		MemoryPoolSize   memorySize;
		uint64_t         occupiedMemory;
		vk::DeviceMemory memory;

		vk::MemoryType memoryType;
		uint32_t memoryTypeIndex;
	};

	/*A very basic/barebone memory allocator, should be used as a singleton.
	It contains pools and pools contain blocks. Pools hold on to device memory and subdivides it into smaller sections contained in blocks*/
	class MemoryAllocator
	{
	public:
		MemoryAllocator(vk::PhysicalDevice physDevice, vk::Device logicDevice)
			: physicalDevice(physDevice),
			device(logicDevice)
		{

		}

		inline SkResult<Pair<uint32_t, uint32_t>, AllocationError> allocateBind(vk::Buffer& buffer, vk::Flags<vk::MemoryPropertyFlagBits> memoryPropertyFlags)
		{
			SkResult result(static_cast<Pair<uint32_t, uint32_t>>(Pair(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max())), static_cast<AllocationError>(AllocationError::NO_ERROR));

			uint32_t createdPoolId = std::numeric_limits<uint32_t>::max();
			uint32_t createdBlockId = std::numeric_limits<uint32_t>::max();

			MemoryPool* currentPool = nullptr;
			if (memoryPools.size() > 0)
			{
				auto getSuitablePoolIdsResult = getSuitablePoolIds(buffer, memoryPropertyFlags);
				std::vector<uint32_t> suitablePoolIds = retLog(getSuitablePoolIdsResult);

				if (suitablePoolIds.size() > 0)
				{
					//@TODO Somehow prefer one pool over another
					uint32_t memoryPoolId = suitablePoolIds[0];
					auto getPoolIdResult = getPoolById(memoryPoolId);
					currentPool = retLog(getPoolIdResult);

				}
				else
				{
					result.error = AllocationError::UNDEFINED_ERROR;
				}

			}
			else
			{
				auto getAppropriateBufferSizeResult = getAppropriateMemorySize(buffer, true);
				MemorySize appropriatePoolSize = retLog(getAppropriateBufferSizeResult);

				auto createMemoryPoolResult = createMemoryPool(static_cast<MemoryPoolSize>(appropriatePoolSize), buffer, memoryPropertyFlags);
				uint32_t memoryPoolId = retLog(createMemoryPoolResult);

				auto getPoolIdResult = getPoolById(memoryPoolId);
				currentPool = retLog(getPoolIdResult);
			}

			auto getAppropriateMemorySizeResult = getAppropriateMemorySize(buffer, false);
			MemorySize appropriateMemorySize = retLog(getAppropriateMemorySizeResult);

			auto createMemoryBlockResult = currentPool->createMemoryBlock(static_cast<MemoryBlockSize>(appropriateMemorySize), buffer);
			uint32_t memoryBlockId = retLog(createMemoryBlockResult);

			auto getBlockByIdResult = currentPool->getBlockById(memoryBlockId);
			MemoryBlock* block = retLog(getBlockByIdResult);

			auto bindResult = currentPool->bind(block, buffer);
			bool binded = retLog(bindResult);

			createdPoolId = retLog(currentPool->getId());
			createdBlockId = memoryBlockId;

			result.value = std::move(Pair(createdPoolId, createdBlockId));
			return result;
		}

		inline SkResult<MemoryPool*, AllocationError> getPoolById(uint32_t poolId)
		{
			SkResult result(static_cast<MemoryPool*>(nullptr), static_cast<AllocationError>(AllocationError::NO_ERROR));

			MemoryPool* memoryPool = nullptr;
			for (uint32_t i = 0; i < memoryPools.size(); i++)
			{
				auto getPoolIdResult = memoryPools[i]->getId();
				uint32_t currPoolId = retLog(getPoolIdResult);

				if (currPoolId == poolId)
				{
					memoryPool = memoryPools[i];
				}
			}

			result.value = std::move(memoryPool);
			return result;
		}

		inline SkResult<uint32_t, AllocationError> getNumberOfPools()
		{
			SkResult result(static_cast<uint32_t>(std::numeric_limits<uint32_t>::max()), static_cast<AllocationError>(AllocationError::NO_ERROR));

			result.value = static_cast<uint32_t>(memoryPools.size());
			return result;
		}

		inline SkResult<bool, AllocationError> freeMemoryPools()
		{
			SkResult result(static_cast<bool>(true), static_cast<AllocationError>(AllocationError::NO_ERROR));

			for (uint32_t i = 0; i < memoryPools.size(); i++)
			{
				delete memoryPools[i];
			}

			return result;
		}

		inline SkResult<bool, AllocationError> freeAllMemory()
		{
			SkResult result(static_cast<bool>(true), static_cast<AllocationError>(AllocationError::NO_ERROR));

			for (uint32_t i = 0; i < memoryPools.size(); i++)
			{
				memoryPools[i]->freeMemory();
				memoryPools[i]->freeMemoryBlocks();
			}

			bool memoryPoolsFreed = retLog(freeMemoryPools());
			if (!memoryPoolsFreed)
			{
				result.value = false;
			}

			return result;
		}
	private:
		vk::PhysicalDevice physicalDevice;
		vk::Device         device;

		std::vector<MemoryPool*> memoryPools = {};

		//Create pool : Allocate memory
		inline SkResult<uint32_t, AllocationError> createMemoryPool(MemoryPoolSize poolSize, vk::Buffer buffer, vk::Flags<vk::MemoryPropertyFlagBits> memoryPropertyFlags)
		{
			SkResult result(static_cast<uint32_t>(std::numeric_limits<uint32_t>::max()), static_cast<AllocationError>(AllocationError::NO_ERROR));

			vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements(buffer);

			auto getMemoryTypeIndexResult = getMemoryTypeIndex(physicalDevice, memoryRequirements.memoryTypeBits, memoryPropertyFlags);
			uint32_t memoryTypeIndex = retLog(getMemoryTypeIndexResult);

			vk::MemoryAllocateInfo memoryAllocateInfo = {};
			memoryAllocateInfo.allocationSize = static_cast<uint64_t>(poolSize);
			memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

			vk::DeviceMemory poolMemory = device.allocateMemory(memoryAllocateInfo);

			uint32_t poolId;
			if (memoryPools.size() > 0)
			{
				auto getLastPoolIdResult = memoryPools[memoryPools.size() - 1]->getId();
				uint32_t lastPoolId = retLog(getLastPoolIdResult);

				poolId = lastPoolId++;
			}
			else
			{
				poolId = 0;
			}

			auto getHeapIndexByTypeIndexResult = getHeapIndexByTypeIndex(physicalDevice, memoryTypeIndex);
			uint32_t poolHeapIndex = retLog(getHeapIndexByTypeIndexResult);

			vk::MemoryType poolMemoryType = {};
			poolMemoryType.heapIndex = poolHeapIndex;
			poolMemoryType.propertyFlags = memoryPropertyFlags;

			MemoryPool* pool = new MemoryPool(device, poolSize, poolId, std::move(poolMemory), poolMemoryType, memoryTypeIndex);
			memoryPools.emplace_back(pool);

			result.value = std::move(poolId);
			return result;
		}

		inline SkResult<bool, AllocationError> isASuitablePool(MemoryPool* pool, vk::Buffer buffer, vk::Flags<vk::MemoryPropertyFlagBits> memoryPropertyFlags)
		{
			SkResult result(static_cast<bool>(true), static_cast<AllocationError>(AllocationError::NO_ERROR));

			vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements(buffer);

			auto getFreePoolMemoryResult = pool->getFreeMemory();
			uint64_t freePoolMemory = retLog(getFreePoolMemoryResult);

			if (static_cast<uint64_t>(memoryRequirements.size) > freePoolMemory)
			{
				result.value = false;
			}

			auto getMemoryTypeIndexResult = getMemoryTypeIndex(physicalDevice, memoryRequirements.memoryTypeBits, memoryPropertyFlags);
			uint32_t memoryTypeIndex = retLog(getMemoryTypeIndexResult);

			auto getPoolMemoryTypeIndex = pool->getMemoryTypeIndex();
			uint32_t poolMemoryTypeIndex = retLog(getPoolMemoryTypeIndex);

			if (memoryTypeIndex != poolMemoryTypeIndex)
			{
				result.value = false;
			}

			auto getPoolMemoryType = pool->getMemoryType();
			vk::MemoryType poolMemoryType = retLog(getPoolMemoryType);

			if (poolMemoryType.propertyFlags != memoryPropertyFlags)
			{
				result.value = false;
			}

			return result;
		}

		inline SkResult<std::vector<uint32_t>, AllocationError> getSuitablePoolIds(vk::Buffer buffer, vk::Flags<vk::MemoryPropertyFlagBits> memoryPropertyFlags)
		{
			SkResult result(static_cast<std::vector<uint32_t>>(std::vector<uint32_t>()), static_cast<AllocationError>(AllocationError::NO_ERROR));

			std::vector<uint32_t> poolIds = {};
			for (uint32_t i = 0; i < memoryPools.size(); i++)
			{
				auto isASuitablePoolResult = isASuitablePool(memoryPools[i], buffer, memoryPropertyFlags);
				bool isSuitable = retLog(isASuitablePoolResult);

				if (isSuitable)
				{
					auto getPoolIdResult = memoryPools[i]->getId();
					uint32_t poolId = retLog(getPoolIdResult);

					poolIds.emplace_back(poolId);
				}
			}

			result.value = std::move(poolIds);
			return result;
		}

		inline SkResult<MemorySize, AllocationError> getAppropriateMemorySize(vk::Buffer buffer, bool isForPool)
		{
			SkResult result(static_cast<MemorySize>(MemorySize::ZERO), static_cast<AllocationError>(AllocationError::NO_ERROR));

			vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements(buffer);

			uint32_t i = 0;
			while (memoryRequirements.size > pow(2, i))
			{
				i++;
			}

			MemorySize appropriatePoolSize;
			if (isForPool)
			{
				appropriatePoolSize = static_cast<MemorySize>(pow(2, i) * 8);
			}
			else
			{
				appropriatePoolSize = static_cast<MemorySize>(pow(2, i));
			}

			result.value = std::move(appropriatePoolSize);
			return result;
		}
	};

	/*Buffer*/

	/*@brief Creates a vertex buffer given a vector of vertices
	*
	* @param device The vulkan device used to create the vertex buffer
	* @param vertices The vector of vertices = content of the buffer
	* @param sharingMode The vulkan sharing mode to be used for the buffer (concurrent vs explicit)
	*
	* @return SkResult(created vertex buffer, BufferError)
	*/
	inline SkResult<vk::Buffer, BufferError> createVertexBuffer(vk::Device device, std::vector<Vertex> vertices, vk::SharingMode sharingMode)
	{
		SkResult result(static_cast<vk::Buffer>(vk::Buffer(nullptr)), static_cast<BufferError>(BufferError::NO_ERROR));

		vk::BufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.size = vertices.size() * sizeof(Vertex);
		bufferCreateInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
		bufferCreateInfo.sharingMode = sharingMode;

		vk::Buffer buffer;
		try
		{
			buffer = device.createBuffer(bufferCreateInfo);
		}
		catch (vk::SystemError err)
		{
			result.error = BufferError::BUFFER_CREATION_ERROR;
		}

		result.value = std::move(buffer);
		return result;
	}

	/*@brief Creates a mesh buffer given a Mesh
	*
	* @param device The vulkan device used to create the mesh buffer
	* @param mesh The content of the buffer
	* @param sharingMode The vulkan sharing mode to be used for the buffer (concurrent vs explicit)
	*
	* @return SkResult(created mesh buffer, BufferError)
	*/
	inline SkResult<vk::Buffer, BufferError> createMeshBuffer(vk::Device device, Mesh mesh, vk::SharingMode sharingMode)
	{
		SkResult result(static_cast<vk::Buffer>(vk::Buffer(nullptr)), static_cast<BufferError>(BufferError::NO_ERROR));

		std::vector<Vertex> meshVertices = mesh.vertices;

		vk::BufferCreateInfo bufferCreateInfo = {};
		bufferCreateInfo.size = meshVertices.size() * sizeof(Vertex);
		bufferCreateInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;
		bufferCreateInfo.sharingMode = sharingMode;

		vk::Buffer buffer;
		try
		{
			buffer = device.createBuffer(bufferCreateInfo);
		}
		catch (vk::SystemError err)
		{
			result.error = BufferError::BUFFER_CREATION_ERROR;
		}

		result.value = std::move(buffer);
		return result;
	}

	/*@brief Destroys a given buffer
	*
	* @param device The vulka device used to destroy the buffer
	* @param buffer The vulkan buffer to be destroyed
	*
	* @return SkResult(boolean indicating if the operation worked(true) or not(false), GraphicsPipelineError)
	*/
	inline SkResult<bool, BufferError> destroyBuffer(vk::Device device, vk::Buffer buffer)
	{
		SkResult result(static_cast<bool>(true), static_cast<BufferError>(BufferError::NO_ERROR));

		try
		{
			device.destroyBuffer(buffer);
		}
		catch (vk::SystemError err)
		{
			result.value = 0;
			result.error = BufferError::BUFFER_DESTRUCTION_ERROR;
		}

		return result;
	}

	/*@brief Creates a basic triangle mesh
	*
	*
	* @return SkResult(created Mesh, MeshError)
	*/
	inline SkResult<Mesh, MeshError> createTriangleMesh()
	{
		SkResult result(static_cast<Mesh>(Mesh()), static_cast<MeshError>(MeshError::NO_ERROR));

		Vec3 pos0(1.0f, 1.0f, 0.0f);
		Vec3 pos1(-1.0f, 1.0f, 0.0f);
		Vec3 pos2(0.0f, -1.0f, 0.0f);

		Vec3 normal0(0.0f, 0.0f, 0.0f);
		Vec3 normal1(0.0f, 0.0f, 0.0f);
		Vec3 normal2(0.0f, 0.0f, 0.0f);

		Vec3 color0(0.0f, 1.0f, 0.0f);
		Vec3 color1(0.0f, 1.0f, 0.0f);
		Vec3 color2(0.0f, 1.0f, 0.0f);

		std::vector<Vertex> vertices;
		vertices.reserve(3); // A triangle has 3 vertices

		vertices.emplace_back(pos0, normal0, color0);

		vertices.emplace_back(pos1, normal1, color1);

		vertices.emplace_back(pos2, normal2, color2);

		result.value = std::move(Mesh(vertices));
		return result;
	}

	/*@brief Creates a basic heart mesh
	*
	*
	* @return SkResult(created Mesh, MeshError)
	*/
	inline SkResult<Mesh, MeshError> createHeartMesh()
	{
		SkResult result(static_cast<Mesh>(Mesh()), static_cast<MeshError>(MeshError::NO_ERROR));

		//Need to invert around y axis in vert shader :(
		Vec3 pos0(0.0f, 0.5f, 0.0f);
		Vec3 pos1(0.5f, 1.0f, 0.0f);
		Vec3 pos2(1.0f, 0.5f, 0.0f);

		Vec3 pos3(0.0f, 0.5f, 0.0f);
		Vec3 pos4(1.0f, 0.5f, 0.0f);
		Vec3 pos5(0.0f, -1.0f, 0.0f);

		Vec3 pos6(-1.0f, 0.5f, 0.0f);
		Vec3 pos7(0.0f, 0.5f, 0.0f);
		Vec3 pos8(0.0f, -1.0f, 0.0f);

		Vec3 pos9(-1.0f, 0.5f, 0.0f);
		Vec3 pos10(-0.5f, 1.0f, 0.0f);
		Vec3 pos11(0.0f, 0.5f, 0.0f);


		Vec3 normal0(0.0f, 0.0f, 0.0f);
		Vec3 normal1(0.0f, 0.0f, 0.0f);
		Vec3 normal2(0.0f, 0.0f, 0.0f);

		Vec3 normal3(0.0f, 0.0f, 0.0f);
		Vec3 normal4(0.0f, 0.0f, 0.0f);
		Vec3 normal5(0.0f, 0.0f, 0.0f);

		Vec3 normal6(0.0f, 0.0f, 0.0f);
		Vec3 normal7(0.0f, 0.0f, 0.0f);
		Vec3 normal8(0.0f, 0.0f, 0.0f);

		Vec3 normal9(0.0f, 0.0f, 0.0f);
		Vec3 normal10(0.0f, 0.0f, 0.0f);
		Vec3 normal11(0.0f, 0.0f, 0.0f);

		Vec3 color0(1.0f, 0.0f, 0.0f);
		Vec3 color1(1.0f, 0.0f, 0.0f);
		Vec3 color2(1.0f, 0.0f, 0.0f);

		Vec3 color3(1.0f, 0.0f, 0.0f);
		Vec3 color4(1.0f, 0.0f, 0.0f);
		Vec3 color5(1.0f, 0.0f, 0.0f);

		Vec3 color6(1.0f, 0.0f, 0.0f);
		Vec3 color7(1.0f, 0.0f, 0.0f);
		Vec3 color8(1.0f, 0.0f, 0.0f);

		Vec3 color9(1.0f, 0.0f, 0.0f);
		Vec3 color10(1.0f, 0.0f, 0.0f);
		Vec3 color11(1.0f, 0.0f, 0.0f);

		std::vector<Vertex> vertices;
		vertices.reserve(12); // A basic heart mesh contains 12 vertices

		vertices.emplace_back(pos0, normal0, color0);

		vertices.emplace_back(pos1, normal1, color1);

		vertices.emplace_back(pos2, normal2, color2);

		vertices.emplace_back(pos3, normal3, color3);

		vertices.emplace_back(pos4, normal4, color4);

		vertices.emplace_back(pos5, normal5, color5);

		vertices.emplace_back(pos6, normal6, color6);

		vertices.emplace_back(pos7, normal7, color7);

		vertices.emplace_back(pos8, normal8, color8);

		vertices.emplace_back(pos9, normal9, color9);

		vertices.emplace_back(pos10, normal10, color10);

		vertices.emplace_back(pos11, normal11, color11);

		result.value = std::move(Mesh(vertices));
		return result;
	}
}
#endif
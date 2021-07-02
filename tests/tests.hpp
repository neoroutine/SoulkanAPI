#ifndef SOULKAN_TESTS_HPP
#define SOULKAN_TESTS_HPP
#pragma once

#include <iostream>
#include <memory>

#include "../Soulkan.hpp"

namespace SOULKAN_NAMESPACE
{
	inline sk::SkResult<sk::SkTestData, sk::TestError> mainSoulkanTest(bool debug)
	{
		sk::SkResult result(sk::SkTestData(), sk::TestError::NO_ERROR);

		/*TEST DATA*/
		auto initStart = std::chrono::steady_clock::now();

		double frameNumber = 0;
		double averageFramesPerSecond = 0;
		double averageFrametime = 0;

		static sk::DeletionQueue deletionQueue;

		/*GLFW*/
		static auto initResult = sk::initGlfw();
		static auto isInit     = sk::retLog(initResult);
		static sk::Window skWindow(800, 600, "Soulkan");
		static GLFWwindow* pWindow = skWindow.get();


		/*INSTANCE*/

		static sk::Instance skInstance;
		if (debug)
		{
			static std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_api_dump" };
			skInstance = sk::Instance("Soulkan", "Soulkan", std::vector<const char*>(), validationLayers);
		}
		else
		{
			skInstance = sk::Instance("Soulkan", "Soulkan");
		}

		static vk::Instance instance = skInstance.get();

		/*PHYSICAL DEVICE*/
		static sk::PhysicalDevice skPhysicalDevice(skInstance);
		static vk::PhysicalDevice physicalDevice = skPhysicalDevice.get();
		std::cout << "Physical device name : " << physicalDevice.getProperties().deviceName << std::endl;

		static vk::SurfaceKHR surface = skInstance.createSurface(skWindow);

		static std::array<uint32_t, 6> queueFamilyIndexes = skPhysicalDevice.getQFIndexes(surface);

		/*DEVICE*/
		static std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		static Device skDevice = skPhysicalDevice.createDevice(queueFamilyIndexes, deviceExtensions);
		static vk::Device device = skDevice.get();

		deletionQueue.push_func([=]() { skDevice.destroy(); });

		/*QUEUE*/
		sk::Queue skQueue = skDevice.getQueue(QueueFamilyType::GENERAL, queueFamilyIndexes, 0);
		static vk::Queue generalQueue = skQueue.get();

		/*SWAPCHAIN*/

		static vk::PresentModeKHR presentMode     = skPhysicalDevice.getAppropriatePresentMode(surface, vk::PresentModeKHR::eMailbox);
		static vk::SurfaceFormatKHR surfaceFormat = skPhysicalDevice.getAppropriateSurfaceFormat(surface);
		static vk::Extent2D extent                = skPhysicalDevice.getAppropriateExtent(surface, skWindow);

		static sk::Swapchain skSwapchain = skDevice.createSwapchain(skWindow, surface, presentMode, surfaceFormat, extent, queueFamilyIndexes);
		static vk::SwapchainKHR swapchain = skSwapchain.get();

		deletionQueue.push_func([=]() { skSwapchain.destroy(); });

		/*COMMAND POOL*/
		static auto createGeneralCommandPoolResult = sk::createCommandPool(device, queueFamilyIndexes, sk::QueueFamilyType::GENERAL);
		static vk::CommandPool generalCommandPool = sk::retLog(createGeneralCommandPoolResult);

		deletionQueue.push_func([=]() { sk::logError(sk::destroyCommandPool(device, generalCommandPool)); });

		/*COMMAND BUFFER*/
		static auto allocateGeneralCommandBufferResult = sk::allocateCommandBuffer(device, generalCommandPool);
		static vk::CommandBuffer generalCommandBuffer = sk::retLog(allocateGeneralCommandBufferResult);

		/*RENDER PASS*/
		static auto createBasicRenderPassResult = sk::createBasicRenderPass(device, surfaceFormat);
		static vk::RenderPass renderPass = sk::retLog(createBasicRenderPassResult);

		deletionQueue.push_func([=]() { sk::logError(sk::destroyRenderPass(device, renderPass)); });

		/*FRAMEBUFFERS*/
		static std::vector<vk::Image> swapchainImages = skSwapchain.getImages();

		static auto createSwapchainImageViewsResult = sk::createSwapchainImageViews(device, swapchainImages, surfaceFormat);
		static std::vector<vk::ImageView> swapchainImageViews = sk::retLog(createSwapchainImageViewsResult);

		deletionQueue.push_func([=]() { sk::logError(sk::destroySwapchainImageViews(device, swapchainImageViews)); });

		static sk::SkResult<std::vector<vk::Framebuffer>, sk::FramebufferError> createFramebuffersResult = sk::createFramebuffers(device, renderPass, 1, extent, swapchain, swapchainImageViews);
		static std::vector<vk::Framebuffer> framebuffers = sk::retLog(createFramebuffersResult);

		deletionQueue.push_func([=]() { sk::logError(sk::destroyFramebuffers(device, framebuffers)); });

		/*SYNCHRONIZATION*/
		static sk::SkResult<vk::Fence, sk::SyncError> createFenceResult = sk::createFence(device);
		static vk::Fence renderFence = sk::retLog(createFenceResult);

		deletionQueue.push_func([=]() { sk::logError(sk::destroyFence(device, renderFence)); });

		static sk::SkResult<vk::Semaphore, sk::SyncError> presentSemaphoreResult = sk::createSemaphore(device);
		static vk::Semaphore presentSemaphore = sk::retLog(presentSemaphoreResult);

		deletionQueue.push_func([=]() { sk::logError(sk::destroySemaphore(device, presentSemaphore)); });

		static sk::SkResult<vk::Semaphore, sk::SyncError> renderSemaphoreResult = sk::createSemaphore(device);
		static vk::Semaphore renderSemaphore = sk::retLog(renderSemaphoreResult);

		deletionQueue.push_func([=]() { sk::logError(sk::destroySemaphore(device, renderSemaphore)); });

		/*GRAPHICS PIPELINE*/

		static auto createPipelineLayoutResult = createPipelineLayout(device);
		static vk::PipelineLayout pipelineLayout = retLog(createPipelineLayoutResult);

		deletionQueue.push_func([=]() { sk::logError(sk::destroyPipelineLayout(device, pipelineLayout)); });

		static std::vector<std::string> shaderFilenames = { "shaders/triangle_mesh.spv", "shaders/colored_triangle.spv" };
		static std::vector<vk::ShaderStageFlagBits> shaderStageFlags = { vk::ShaderStageFlagBits::eVertex, vk::ShaderStageFlagBits::eFragment };
		static std::vector<std::string> entryNames = { "main", "main" };

		/*SHADERS*/
		static auto vertexShaderModuleResult = createShaderModule(device, shaderFilenames[0]);
		static vk::ShaderModule vertexShaderModule = retLog(vertexShaderModuleResult);
		static auto fragmentShaderModuleResult = createShaderModule(device, shaderFilenames[1]);
		static vk::ShaderModule fragmentShaderModule = retLog(fragmentShaderModuleResult);

		static std::vector<vk::ShaderModule> shaderModules = { vertexShaderModule, fragmentShaderModule };

		deletionQueue.push_func([=]() { sk::logError(sk::destroyShaderModule(device, vertexShaderModule)); });
		deletionQueue.push_func([=]() { sk::logError(sk::destroyShaderModule(device, fragmentShaderModule)); });

		/*MESH*/
		static auto createTriangleMeshResult = sk::createTriangleMesh();
		static sk::Mesh triangleMesh = sk::retLog(createTriangleMeshResult);

		std::vector<sk::Vertex> triangleMeshVertices = triangleMesh.vertices;

		/*VERTEX INPUT DESCRIPTIONS*/
		static auto vertexInputBindingDescriptionResult = triangleMeshVertices[0].getInputBindingDescription();
		static vk::VertexInputBindingDescription vertexInputBindingDescription = sk::retLog(vertexInputBindingDescriptionResult);
		static std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions = { vertexInputBindingDescription };

		static auto vertexInputAttributeDescriptionsResult = triangleMeshVertices[0].getInputAttributeDescriptions();
		static std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions = sk::retLog(vertexInputAttributeDescriptionsResult);

		static auto createGraphicsPipelineResult = createGraphicsPipeline(device, renderPass, pipelineLayout, extent, shaderStageFlags, shaderModules, entryNames, vertexInputBindingDescriptions, vertexInputAttributeDescriptions);
		static vk::Pipeline graphicsPipeline    = retLog(createGraphicsPipelineResult);

		deletionQueue.push_func([=]() { sk::logError(sk::destroyPipeline(device, graphicsPipeline)); });

		/*BUFFER*/
		static auto createVertexBufferResult = sk::createVertexBuffer(device, triangleMeshVertices, vk::SharingMode::eExclusive);
		static vk::Buffer vertexBuffer = sk::retLog(createVertexBufferResult);
		static std::vector<vk::Buffer> vertexBuffers = { vertexBuffer };

		deletionQueue.push_func([=]() { sk::logError(sk::destroyBuffer(device, vertexBuffer)); });

		/*MEMORY ALLOCATOR*/
		static sk::MemoryAllocator* pAllocator = new sk::MemoryAllocator(physicalDevice, device);

		deletionQueue.push_func([=]() { sk::logError(pAllocator->freeAllMemory()); });
		deletionQueue.push_func([=]() { delete pAllocator; });

		static auto allocateBindResult = pAllocator->allocateBind(vertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostVisible);
		static sk::Pair<uint32_t, uint32_t> poolIdBlockId = sk::retLog(allocateBindResult);

		static auto getPoolByIdResult = pAllocator->getPoolById(poolIdBlockId.a);
		static sk::MemoryPool* pPool = sk::retLog(getPoolByIdResult);

		static auto getBlockByIdResult = pPool->getBlockById(poolIdBlockId.b);
		static sk::MemoryBlock* pBlock = sk::retLog(getBlockByIdResult);

		static auto mapResult = pPool->map(pBlock);
		static void* pData = sk::retLog(mapResult);

		memcpy(pData, triangleMeshVertices.data(), triangleMeshVertices.size() * sizeof(Vertex));

		static auto unmapResult = pPool->unmap();

		std::vector<sk::Vertex> verticesToBeDrawn = std::move(triangleMeshVertices);

		skm::Mat4 translation = skm::translation(1.f, 2.f, 3.f);
		std::cout << translation.asString() << std::endl;

		std::cout << "45 deg  : " << skm::toRad(45.f)  << " rad" << std::endl;
		std::cout << "90 deg  : " << skm::toRad(90.f)  << " rad" << std::endl;
		std::cout << "135 deg : " << skm::toRad(135.f) << " rad" << std::endl;
		std::cout << "180 deg : " << skm::toRad(180.f) << " rad" << std::endl;
		std::cout << "360 deg : " << skm::toRad(360.f) << " rad" << std::endl;

		auto initEnd = std::chrono::steady_clock::now();
		auto frameStart = std::chrono::steady_clock::now();
		while (!glfwWindowShouldClose(pWindow))
		{
			glfwPollEvents();

			sk::draw(device, generalCommandBuffer, swapchain, generalQueue, graphicsPipeline, renderFence, presentSemaphore, renderSemaphore, renderPass, extent, framebuffers, vertexBuffers, verticesToBeDrawn, frameNumber);

			auto now = std::chrono::steady_clock::now();
			std::chrono::duration<double> delta = now - frameStart;

			double framesPerSecond = skm::retLog(skm::getFramePerSecond(frameNumber, delta));
			double frametime       = skm::retLog(skm::getFrametime(framesPerSecond));

			averageFramesPerSecond += framesPerSecond;
			averageFrametime += frametime;

			std::string title = ("Frame " + std::to_string(frameNumber).substr(0, 10) + " (" + std::to_string(framesPerSecond).substr(0, 5) + 
				                          " fps or frametime : " + std::to_string(frametime).substr(0, 5) + " ms) DEBUG : ");
			title += (debug ? "ON" : "OFF");

			glfwSetWindowTitle(pWindow, title.c_str());
		}

		averageFramesPerSecond /= frameNumber;
		averageFrametime /= frameNumber;

		auto testEnd = std::chrono::steady_clock::now();
		auto delta   = testEnd - initStart;
		double timeElapsed = delta.count() / 1000000000.0;

		auto delta2 = initEnd - initStart;
		double initTimeElapsed = delta2.count() / 1000000000.0;


		device.waitIdle();

		/*Cleanup stuff*/

		pAllocator->freeAllMemory();

		deletionQueue.flush();

		result.value = std::move(sk::SkTestData(frameNumber, timeElapsed, initTimeElapsed, averageFramesPerSecond, averageFrametime));
		result.value.initTimeElapsed = initTimeElapsed;
		return result;
	}
}
#endif
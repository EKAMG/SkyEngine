//
// Created by Zach Lee on 2021/12/26.
//

#include <engine/render/rendergraph/ForwardRendering.h>
#include <engine/render/rendergraph/RenderGraph.h>
#include <engine/render/rendergraph/RenderGraphPassData.h>
#include <engine/render/DriverManager.h>
#include <vulkan/Util.h>

namespace sky {

    ForwardRendering::ForwardRendering()
    {
    }

    ForwardRendering::~ForwardRendering()
    {
    }

    void ForwardRendering::SetupImage()
    {
        auto device = DriverManager::Get()->GetDevice();

        auto& ext = swapChain->GetExtent();
        if (ext.width != extent.width || ext.height != extent.height || !depthImage) {
            auto desc = drv::MakeImage2D(VK_FORMAT_D32_SFLOAT_S8_UINT, ext);
            desc.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            desc.memory = VMA_MEMORY_USAGE_GPU_ONLY;
            depthImage = device->CreateDeviceObject<drv::Image>(desc);
        }
        extent = ext;
    }

    void ForwardRendering::Render(RenderGraph& renderGraph)
    {
        SetupImage();

        renderGraph.AddPass<GraphicPassData>("ForwardColor",
            [this](RenderGraphBuilder& builder, GraphicPassData& data) -> bool {
            builder.ImportImage("MainColor", swapChain->GetImage());
            builder.ImportImage("MainDepthStencil", depthImage);
            auto color = builder.WriteImage("MainColor", drv::ImageView::Make2DColor(swapChain->GetFormat()),
                ImageBindingFlag{ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT },
                AttachmentDesc{VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE});
            color->SetClearColor(drv::MakeClearColor(1.0, 0.0, 0.0, 1.0));
            data.colors.emplace_back(color);
            data.depthStencil = builder.WriteImage("MainDepthStencil", drv::ImageView::Make2DDepthStencil(VK_FORMAT_D32_SFLOAT_S8_UINT),
                ImageBindingFlag{VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT},
                AttachmentDesc{VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE});
            data.extent2D = swapChain->GetExtent();
            BuildGraphicsPass(data);
            return true;
        }, [](GraphicPassData& data, const RenderGraph&, drv::CommandBuffer& cmdBuffer) {
            BuildFrameBuffer(data);
            VkRenderPassBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            beginInfo.pNext = nullptr;
            beginInfo.renderPass = data.pass->GetNativeHandle();
            beginInfo.framebuffer = data.frameBuffer->GetNativeHandle();
            beginInfo.renderArea.extent = data.extent2D;
            beginInfo.clearValueCount = static_cast<uint32_t>(data.clears.size());
            beginInfo.pClearValues = data.clears.data();
            auto cmd = cmdBuffer.GetNativeHandle();
            vkCmdBeginRenderPass(cmd, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdEndRenderPass(cmd);
        });

        renderGraph.AddPass<GraphicPassData>("SwapChain",
        [this](RenderGraphBuilder& builder, GraphicPassData& data) -> bool {
            auto color = builder.WriteImage("MainColor", drv::ImageView::Make2DColor(swapChain->GetFormat()),
                ImageBindingFlag{ VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT },
                AttachmentDesc{VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE});
            color->SetClearColor(drv::MakeClearColor(0.0, 1.0, 0.0, 1.0));
            data.colors.emplace_back(color);
            data.extent2D = swapChain->GetExtent();
            BuildGraphicsPass(data);
            return true;
        }, [](GraphicPassData& data, const RenderGraph&, drv::CommandBuffer& cmdBuffer) {
            BuildFrameBuffer(data);
            VkRenderPassBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            beginInfo.pNext = nullptr;
            beginInfo.renderPass = data.pass->GetNativeHandle();
            beginInfo.framebuffer = data.frameBuffer->GetNativeHandle();
            beginInfo.renderArea.extent = data.extent2D;
            beginInfo.clearValueCount = static_cast<uint32_t>(data.clears.size());
            beginInfo.pClearValues = data.clears.data();
            auto cmd = cmdBuffer.GetNativeHandle();
            vkCmdBeginRenderPass(cmd, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdEndRenderPass(cmd);
        });

//        renderGraph.AddPass<GraphicPassData>("ForwardColor",
//            [this](RenderGraphBuilder& builder, GraphicPassData& data) -> bool {
//            drv::Image::Descriptor desc = {};
//            auto& ext = swapChain->GetExtent();
//            desc.extent = VkExtent3D{ext.width, ext.height, 1};
//            desc.format = VK_FORMAT_D32_SFLOAT_S8_UINT;
//            desc.memory = VMA_MEMORY_USAGE_GPU_ONLY;
//            desc.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
//            desc.transient = false;
//            builder.CreateImage("MainDepthStencilImage", desc);
//
//            desc.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
//            desc.format = swapChain->GetFormat();
//            builder.CreateImage("MainColorImage", desc);
//
//            drv::ImageView::Descriptor viewDesc = {};
//            viewDesc.format = swapChain->GetFormat();
//            viewDesc.subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//            data.clears.emplace_back(drv::MakeClearColor(1.f, 0.f, 0.f, 0.f));
//            data.attachments.emplace_back(builder.Write("MainColorImage", viewDesc));
//
//            viewDesc.format = VK_FORMAT_D32_SFLOAT_S8_UINT;
//            viewDesc.subResourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
//            data.clears.emplace_back(drv::MakeClearDepthStencil(1.f, 0));
//            data.attachments.emplace_back(builder.Write("MainDepthStencilImage", viewDesc));
//
//            auto& pass = passes["ForwardColor"];
//            data.fbInfo.pass = pass.get();
//            data.fbInfo.extent = ext;
//            return true;
//        }, [](GraphicPassData& data, const RenderGraph&, drv::CommandBuffer& cmd) {
//            cmd.Encode([&data](VkCommandBuffer commandBuffer) {
//                if (data.fbInfo.pass == nullptr) {
//                    return;
//                }
//                data.fbInfo.views.resize(data.attachments.size());
//                for (uint32_t i = 0; i < data.attachments.size(); ++i) {
//                    auto& view = data.attachments[i]->GetImageView();
//                    if (!view) {
//                        return;
//                    }
//                    data.fbInfo.views[i] = view->GetNativeHandle();
//                }
//
//                data.frameBuffer = DriverManager::Get()->GetDevice()->CreateDeviceObject<drv::FrameBuffer>(data.fbInfo);
//
//                VkRenderPassBeginInfo beginInfo = {};
//                beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//                beginInfo.pNext = nullptr;
//                beginInfo.renderPass = data.fbInfo.pass->GetNativeHandle();
//                beginInfo.framebuffer = data.frameBuffer->GetNativeHandle();
//                beginInfo.renderArea.extent = data.frameBuffer->GetExtent();
//                beginInfo.clearValueCount = static_cast<uint32_t>(data.clears.size());
//                beginInfo.pClearValues = data.clears.data();
//
//                vkCmdBeginRenderPass(commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
//
//                vkCmdEndRenderPass(commandBuffer);
//            });
//        });
//
//        renderGraph.AddPass<SwapChainPassData>("SwapChain", [this](RenderGraphBuilder& builder, SwapChainPassData& data) -> bool {
//            data.swapChain = swapChain;
//            data.image = builder.ReadImage("MainColorImage");
//            builder.SideEffect();
//            return true;
//        }, [](SwapChainPassData& data, const RenderGraph&, drv::CommandBuffer& cmd) {
//            auto dst = data.swapChain->GetImage();
//            auto src = data.image;
//
//            auto srcImage = src->GetImage()->GetNativeHandle();
//            auto dstImage = dst->GetNativeHandle();
//
//            VkImageMemoryBarrier barrier = {};
//            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//            barrier.pNext = nullptr;
//            barrier.srcQueueFamilyIndex = 0;
//            barrier.dstQueueFamilyIndex = 0;
//            barrier.subresourceRange = {
//                VK_IMAGE_ASPECT_COLOR_BIT,
//                0, 1, 0, 1
//            };
//
//            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
//            barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
//            barrier.image = srcImage;
//            cmd.Barrier(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, barrier);
//
//            barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//            barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
//            barrier.image = dstImage;
//            cmd.Barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, barrier);
//
//            VkImageCopy copy = {};
//            copy.extent = src->GetImage()->GetImageInfo().extent;
//            copy.srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
//            copy.dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
//            cmd.Copy(srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, copy);
//
//            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//            barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
//            barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//            barrier.image = dstImage;
//            cmd.Barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, barrier);
//
//            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
//            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
//            barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//            barrier.image = srcImage;
//            cmd.Barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, barrier);
//        });

    }

}
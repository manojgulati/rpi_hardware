#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbd4a4d5c, "module_layout" },
	{ 0x63ce659b, "param_ops_int" },
	{ 0x4980316c, "v4l2_event_unsubscribe" },
	{ 0x5ccbb968, "vb2_ioctl_streamoff" },
	{ 0xf849e827, "vb2_ioctl_streamon" },
	{ 0x15d00c47, "vb2_ioctl_prepare_buf" },
	{ 0x625c356f, "vb2_ioctl_create_bufs" },
	{ 0x2038f2b0, "vb2_ioctl_dqbuf" },
	{ 0xd6df8f5f, "vb2_ioctl_expbuf" },
	{ 0x75f04ff7, "vb2_ioctl_qbuf" },
	{ 0xed2637b8, "vb2_ioctl_querybuf" },
	{ 0x38501d89, "vb2_ioctl_reqbufs" },
	{ 0xe0a8150b, "vb2_fop_mmap" },
	{ 0x383ae5e, "video_ioctl2" },
	{ 0xd1a2841, "vb2_fop_poll" },
	{ 0xec72c4cc, "vb2_fop_read" },
	{ 0xe007d351, "vb2_ops_wait_finish" },
	{ 0x326251bc, "vb2_ops_wait_prepare" },
	{ 0x3c220355, "platform_driver_unregister" },
	{ 0xb2644c66, "__platform_driver_register" },
	{ 0xa8d01b8f, "_dev_err" },
	{ 0x5df55ea4, "pm_runtime_enable" },
	{ 0xe418b758, "of_node_put" },
	{ 0x3f77a198, "v4l2_async_notifier_register" },
	{ 0xff8cc227, "v4l2_async_notifier_add_subdev" },
	{ 0xf350355, "v4l2_async_notifier_init" },
	{ 0xddc8ddc3, "v4l2_fwnode_endpoint_parse" },
	{ 0x587ac4f9, "of_graph_get_remote_port_parent" },
	{ 0xfc838895, "of_graph_get_next_endpoint" },
	{ 0x85fb927f, "of_property_read_variable_u32_array" },
	{ 0x634a8148, "v4l2_ctrl_handler_init_class" },
	{ 0x891ffb48, "__media_device_register" },
	{ 0xafec8c43, "v4l2_device_register" },
	{ 0x6e9f47d8, "media_device_init" },
	{ 0xf5f04c87, "devm_request_threaded_irq" },
	{ 0x9e4b44c1, "platform_get_irq" },
	{ 0xb5e8f9f9, "devm_clk_get" },
	{ 0x875498b, "devm_platform_ioremap_resource" },
	{ 0x6b37f363, "kmem_cache_alloc_trace" },
	{ 0x85a63e63, "kmalloc_caches" },
	{ 0x594640ef, "__v4l2_device_register_subdev_nodes" },
	{ 0x4c773df1, "v4l2_subdev_alloc_pad_config" },
	{ 0x76d9b876, "clk_set_rate" },
	{ 0x815588a6, "clk_enable" },
	{ 0x7c9a7371, "clk_prepare" },
	{ 0x950ff5a1, "__pm_runtime_resume" },
	{ 0x296695f, "refcount_warn_saturate" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0x53d74b97, "media_create_pad_link" },
	{ 0x82620c75, "__video_register_device" },
	{ 0x80b34263, "dma_alloc_attrs" },
	{ 0x6a83e9b9, "media_entity_pads_init" },
	{ 0xa75b9948, "vb2_queue_init" },
	{ 0x4963ccde, "vb2_dma_contig_memops" },
	{ 0x9324e19f, "v4l2_ctrl_add_handler" },
	{ 0xe346f67a, "__mutex_init" },
	{ 0x6623c047, "v4l2_event_subscribe" },
	{ 0xce4fe70e, "v4l2_ctrl_subscribe_event" },
	{ 0x37a0cba, "kfree" },
	{ 0x1cd1e10e, "v4l2_subdev_free_pad_config" },
	{ 0xcdb20441, "media_device_cleanup" },
	{ 0xd3854084, "v4l2_ctrl_handler_free" },
	{ 0x2e326fe8, "__pm_runtime_idle" },
	{ 0xb077e70a, "clk_unprepare" },
	{ 0xb6e6d99d, "clk_disable" },
	{ 0x25379e73, "clk_set_min_rate" },
	{ 0x12a38747, "usleep_range" },
	{ 0x124ed275, "v4l2_fh_release" },
	{ 0x97e8ddd7, "v4l2_fh_open" },
	{ 0x67ea780, "mutex_unlock" },
	{ 0xecf82b67, "_vb2_fop_release" },
	{ 0xd0a13ef2, "v4l2_fh_is_singular" },
	{ 0xc271c3be, "mutex_lock" },
	{ 0xd034392d, "v4l2_match_dv_timings" },
	{ 0xf5ef842e, "v4l_bound_align_image" },
	{ 0x368aae9a, "v4l2_g_parm_cap" },
	{ 0x9f6438e9, "v4l2_s_parm_cap" },
	{ 0xc358aaf8, "snprintf" },
	{ 0xeea0399, "strscpy" },
	{ 0x9d669763, "memcpy" },
	{ 0x5f8ae24, "video_devdata" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0xae577d60, "_raw_spin_lock" },
	{ 0xb43f9365, "ktime_get" },
	{ 0xffb44108, "vb2_plane_cookie" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xf0636004, "v4l2_event_queue" },
	{ 0x19004b96, "vb2_buffer_done" },
	{ 0x92091f96, "__pm_runtime_disable" },
	{ 0x1e739ac4, "media_device_unregister" },
	{ 0xb7570b42, "v4l2_device_unregister" },
	{ 0xc217ea5f, "v4l2_async_notifier_unregister" },
	{ 0x6a3acb2c, "video_unregister_device" },
	{ 0x838edad9, "dma_free_attrs" },
	{ 0xc5850110, "printk" },
	{ 0xf3d0b495, "_raw_spin_unlock_irqrestore" },
	{ 0xde55e795, "_raw_spin_lock_irqsave" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x5f754e5a, "memset" },
	{ 0xdc11406f, "v4l2_subdev_call_wrappers" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "videodev,videobuf2-v4l2,v4l2-fwnode,mc,videobuf2-dma-contig,v4l2-dv-timings,videobuf2-common");

MODULE_ALIAS("of:N*T*Cbrcm,bcm2835-unicam");
MODULE_ALIAS("of:N*T*Cbrcm,bcm2835-unicamC*");

MODULE_INFO(srcversion, "CB55BF1B04F10C4B7BBBAC9");

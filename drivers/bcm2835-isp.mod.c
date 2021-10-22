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
	{ 0x38501d89, "vb2_ioctl_reqbufs" },
	{ 0x1d4149cd, "vchiq_mmal_port_set_format" },
	{ 0x85a63e63, "kmalloc_caches" },
	{ 0x4980316c, "v4l2_event_unsubscribe" },
	{ 0xf8d74e83, "vchiq_mmal_port_disable" },
	{ 0x9618fffc, "video_device_release_empty" },
	{ 0xd01bfc93, "media_device_unregister_entity" },
	{ 0x9460bf84, "media_devnode_create" },
	{ 0x6862abfe, "vchiq_mmal_port_parameter_set" },
	{ 0xed92a76, "vb2_wait_for_all_buffers" },
	{ 0xb7570b42, "v4l2_device_unregister" },
	{ 0xd3854084, "v4l2_ctrl_handler_free" },
	{ 0xe9ed4584, "v4l2_ctrl_new_std" },
	{ 0xd1a2841, "vb2_fop_poll" },
	{ 0xf849e827, "vb2_ioctl_streamon" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x82620c75, "__video_register_device" },
	{ 0xeea0399, "strscpy" },
	{ 0xb2644c66, "__platform_driver_register" },
	{ 0xafec8c43, "v4l2_device_register" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0x65983e03, "media_remove_intf_links" },
	{ 0x81e98b17, "vchiq_mmal_component_finalise" },
	{ 0x5f754e5a, "memset" },
	{ 0xe0a8150b, "vb2_fop_mmap" },
	{ 0x75f04ff7, "vb2_ioctl_qbuf" },
	{ 0x6623c047, "v4l2_event_subscribe" },
	{ 0xe346f67a, "__mutex_init" },
	{ 0xc5850110, "printk" },
	{ 0x6a3acb2c, "video_unregister_device" },
	{ 0x6a83e9b9, "media_entity_pads_init" },
	{ 0x7d60cb0c, "vc_sm_cma_import_dmabuf" },
	{ 0xce4fe70e, "v4l2_ctrl_subscribe_event" },
	{ 0x95f2a2d2, "vb2_plane_vaddr" },
	{ 0x19004b96, "vb2_buffer_done" },
	{ 0x2062ab9d, "dma_buf_put" },
	{ 0x3a93f15e, "dma_buf_get" },
	{ 0x15e16d8c, "vchiq_mmal_component_enable" },
	{ 0x15d00c47, "vb2_ioctl_prepare_buf" },
	{ 0x779a5bc, "v4l2_ctrl_new_custom" },
	{ 0x625c356f, "vb2_ioctl_create_bufs" },
	{ 0x2038f2b0, "vb2_ioctl_dqbuf" },
	{ 0x73577d20, "vchiq_mmal_finalise" },
	{ 0xcdb20441, "media_device_cleanup" },
	{ 0x78c04582, "media_device_register_entity" },
	{ 0x32ee9912, "vchiq_mmal_component_init" },
	{ 0x3fd58a47, "vb2_fop_release" },
	{ 0x5f8ae24, "video_devdata" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x8d9c023c, "vchiq_mmal_component_disable" },
	{ 0xcae690b3, "mmal_vchi_buffer_init" },
	{ 0x53d74b97, "media_create_pad_link" },
	{ 0x9b0d511e, "media_entity_remove_links" },
	{ 0x1d8ba8c3, "media_create_intf_link" },
	{ 0x6b37f363, "kmem_cache_alloc_trace" },
	{ 0x97e8ddd7, "v4l2_fh_open" },
	{ 0xf1e14262, "vchiq_mmal_port_enable" },
	{ 0xed2637b8, "vb2_ioctl_querybuf" },
	{ 0x891ffb48, "__media_device_register" },
	{ 0x53b52981, "vc_sm_cma_free" },
	{ 0x4963ccde, "vb2_dma_contig_memops" },
	{ 0x37a0cba, "kfree" },
	{ 0x3440f59, "vchiq_mmal_submit_buffer" },
	{ 0x634a8148, "v4l2_ctrl_handler_init_class" },
	{ 0xaca4dd80, "vchiq_mmal_init" },
	{ 0x870d5a1c, "__init_swait_queue_head" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0xc37335b0, "complete" },
	{ 0xc358aaf8, "snprintf" },
	{ 0x6e9f47d8, "media_device_init" },
	{ 0xd6df8f5f, "vb2_ioctl_expbuf" },
	{ 0xa4e0c1cf, "mmal_vchi_buffer_cleanup" },
	{ 0x3c220355, "platform_driver_unregister" },
	{ 0x5ccbb968, "vb2_ioctl_streamoff" },
	{ 0xade12dec, "devm_kmalloc" },
	{ 0xa16b21fb, "wait_for_completion_timeout" },
	{ 0xea5d17a0, "vb2_queue_release" },
	{ 0x7973b720, "param_ops_uint" },
	{ 0x6fa2a386, "vc_sm_cma_int_handle" },
	{ 0x1e739ac4, "media_device_unregister" },
	{ 0x383ae5e, "video_ioctl2" },
	{ 0xd7850001, "vb2_core_expbuf_dmabuf" },
	{ 0xc6705900, "media_devnode_remove" },
	{ 0xe07207af, "vchiq_mmal_port_parameter_get" },
	{ 0xa75b9948, "vb2_queue_init" },
};

MODULE_INFO(depends, "videobuf2-v4l2,bcm2835-mmal-vchiq,videodev,mc,videobuf2-common,vc-sm-cma,videobuf2-dma-contig");


MODULE_INFO(srcversion, "18DFCD34CF466DBDF231487");

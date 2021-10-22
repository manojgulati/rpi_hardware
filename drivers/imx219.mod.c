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
	{ 0x6bd78d36, "v4l2_event_subdev_unsubscribe" },
	{ 0xbf95388e, "v4l2_ctrl_subdev_subscribe_event" },
	{ 0xd002b257, "i2c_del_driver" },
	{ 0xb7a49f4a, "i2c_register_driver" },
	{ 0x1c0edc57, "__v4l2_ctrl_s_ctrl" },
	{ 0xff76573f, "__v4l2_find_nearest_size" },
	{ 0x769f1102, "__v4l2_ctrl_grab" },
	{ 0xf95da2b0, "__v4l2_ctrl_handler_setup" },
	{ 0x950ff5a1, "__pm_runtime_resume" },
	{ 0x5df55ea4, "pm_runtime_enable" },
	{ 0xcc951e13, "v4l2_async_register_subdev_sensor_common" },
	{ 0x6a83e9b9, "media_entity_pads_init" },
	{ 0x624652e9, "v4l2_ctrl_new_fwnode_properties" },
	{ 0x56d75a08, "v4l2_fwnode_device_parse" },
	{ 0x81c46254, "v4l2_ctrl_new_std_menu_items" },
	{ 0xe9ed4584, "v4l2_ctrl_new_std" },
	{ 0xe346f67a, "__mutex_init" },
	{ 0x634a8148, "v4l2_ctrl_handler_init_class" },
	{ 0x131d8e8e, "i2c_transfer" },
	{ 0x18ce880d, "devm_gpiod_get_optional" },
	{ 0xe9ea9532, "devm_regulator_bulk_get" },
	{ 0x556e4390, "clk_get_rate" },
	{ 0xb5e8f9f9, "devm_clk_get" },
	{ 0xf98e4404, "fwnode_handle_put" },
	{ 0x89d44b77, "v4l2_fwnode_endpoint_free" },
	{ 0x4f4af446, "v4l2_fwnode_endpoint_alloc_parse" },
	{ 0x12cc4af6, "fwnode_graph_get_next_endpoint" },
	{ 0x2d3b1846, "dev_fwnode" },
	{ 0x5f754e5a, "memset" },
	{ 0xb60ffda1, "v4l2_i2c_subdev_init" },
	{ 0xade12dec, "devm_kmalloc" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0x67ea780, "mutex_unlock" },
	{ 0xc271c3be, "mutex_lock" },
	{ 0xf4e9aa96, "__pm_runtime_set_status" },
	{ 0x92091f96, "__pm_runtime_disable" },
	{ 0xd3854084, "v4l2_ctrl_handler_free" },
	{ 0xbedbf0f9, "v4l2_async_unregister_subdev" },
	{ 0xe471ab95, "_dev_info" },
	{ 0x1952ac80, "__v4l2_ctrl_modify_range" },
	{ 0x2e326fe8, "__pm_runtime_idle" },
	{ 0x2b72d6e, "pm_runtime_get_if_active" },
	{ 0xee43fd9b, "___ratelimit" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0xfaa9b28e, "i2c_transfer_buffer_flags" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xb6e6d99d, "clk_disable" },
	{ 0xb077e70a, "clk_unprepare" },
	{ 0xc5850110, "printk" },
	{ 0x12a38747, "usleep_range" },
	{ 0xcb4d560, "gpiod_set_value_cansleep" },
	{ 0x815588a6, "clk_enable" },
	{ 0x8383d82, "regulator_bulk_disable" },
	{ 0xa8d01b8f, "_dev_err" },
	{ 0x7c9a7371, "clk_prepare" },
	{ 0xcf225a81, "regulator_bulk_enable" },
	{ 0xb43f9365, "ktime_get" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "videodev,v4l2-fwnode,mc");

MODULE_ALIAS("of:N*T*Csony,imx219");
MODULE_ALIAS("of:N*T*Csony,imx219C*");

MODULE_INFO(srcversion, "59C9D6DCE84F4878F974446");

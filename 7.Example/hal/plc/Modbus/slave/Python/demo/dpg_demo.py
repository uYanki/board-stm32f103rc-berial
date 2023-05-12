import dearpygui.dearpygui as dpg
from dearpygui import demo

dpg.create_context()
dpg.create_viewport(title="modbus", width=600, height=400)
dpg.setup_dearpygui()

demo.show_demo()

dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()

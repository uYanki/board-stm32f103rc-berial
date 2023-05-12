from dearpygui import dearpygui as dpg

from math import ceil


def save_callback():
    print("Save Clicked")


dpg.create_context()
dpg.create_viewport(title="modbus", width=600, height=400)
dpg.setup_dearpygui()

dat = [i for i in range(105)]


def _log(sender, app_data, user_data):
    print(f"sender: {sender}, \t app_data: {app_data}, \t user_data: {user_data}")


with dpg.window(label="data view", width=400, height=450) as w:

    addr = 0
    dpg.add_text("slave:")

    dpg.add_same_line()
    slave_id = dpg.add_input_int(default_value=1, min_value=1, max_value=255, min_clamped=True, max_clamped=True, width=-1)

    types = ["Coils", "Input Registers", "Holding Registers", "Discrete Inputs"]
    dpg.add_text("type:")
    dpg.add_same_line()
    dpg.add_combo(default_value=types[0], items=types, width=-1)

    dpg.add_text("addr:")
    dpg.add_same_line()
    dpg.add_input_int(width=-1, callback=lambda s, a, u: print(a))

    dpg.add_text("count:")
    dpg.add_same_line()
    dpg.add_drag_int(default_value=1, min_value=1, max_value=100, clamped=True, width=-1)

    dpg.add_separator()

    dpg.add_text("columns:")
    dpg.add_same_line()
    dpg.add_drag_int(default_value=1, min_value=1, max_value=100, clamped=True, width=-1)

    with dpg.table(header_row=True, borders_innerH=True, borders_innerV=True, freeze_rows=1, freeze_columns=1,
                   scrollY=True, scrollX=True, policy=dpg.mvTable_SizingFixedSame) as id_table:

        dat_idx = 0
        dat_cnt = len(dat)
        dat_addr = 1000

        col_cnt = 10
        row_cnt = ceil(dat_cnt / col_cnt)

        dpg.add_table_column(label="")
        for col in range(0, col_cnt):
            dpg.add_table_column(label=f"{col}")

        for row in range(row_cnt):
            with dpg.table_row():
                # address
                dpg.add_text(f"{dat_addr}", color=[255, 255, 0])
                dat_addr += col_cnt
                # content
                for col in range(1, min(col_cnt, dat_cnt-dat_idx)+1):
                    dpg.add_text(f"{dat[dat_idx]}")
                    dpg.highlight_table_cell(id_table, row=row, column=col, color=[0, 0, 0] if dat[dat_idx] == 0 else [0, 128, 0])
                    dat_idx += 1

dpg.show_viewport()
if dpg.is_viewport_ok():
    while dpg.is_dearpygui_running():
        dpg.render_dearpygui_frame()
dpg.destroy_context()


from serial import Serial
import modbus_tk.defines as modbus_cst
from modbus_tk import modbus_rtu

from dearpygui import dearpygui as dpg


from threading import Timer

''' init interface '''

try:
    serial = Serial(port="com11", baudrate=115200, bytesize=8, parity='N', stopbits=1)
    master = modbus_rtu.RtuMaster(serial)
    master.set_timeout(1.0)
    # master.set_verbose(True)
except Exception as exc:
    print(str(exc))
    exit()

''' event handler '''


bufidx = 0
bufcap = 1000  # 缓冲大小
bufnop = 200
series_x = [i for i in range(bufcap)]
series_y = [0 for _ in range(bufcap)]

interval = 0.05

success_count = 0
error_count = 0


dpg.create_context()
dpg.create_viewport(title="modbus", width=600, height=400)
dpg.setup_dearpygui()

with dpg.window(collapsed=False) as w:
    dpg.set_primary_window(w, True)
    dpg.add_text(default_value="error(0) / success(0)",  tag="lbl_errcnt")
    with dpg.plot(height=-1, width=-1):
        dpg.add_plot_axis(dpg.mvXAxis, tag="x-axis", label="index")
        with dpg.plot_axis(dpg.mvYAxis, tag="y-axis", label="value"):
            dpg.add_area_series(series_x, series_y, tag="series_data")


def read_new_data():

    # slave params
    slave = 0x0A
    address = 1004

    # read data

    global error_count, success_count

    try:
        words = master.execute(slave, modbus_cst.READ_INPUT_REGISTERS, address, 1)
        print(words)

        global series_x, series_y, bufidx, bufcap

        # index loop
        if bufidx == bufcap:
            bufidx = 0
        # zero buffer
        elif bufidx < bufcap-bufnop:
            series_y[bufidx+bufnop] = 0

        # print(words)
        series_y[bufidx] = words[0]
        bufidx += 1
        # update plot
        dpg.set_value("series_data", [series_x, series_y])

        success_count += 1

    except:
        error_count += 1

    dpg.set_value("lbl_errcnt", f"error({error_count}) / success({success_count})")

    # restart
    global interval
    if interval:
        # Timer(interval, read_new_data).start()
        pass

# timer = Timer(interval, read_new_data)

# run


dpg.show_viewport()
# timer.start()
# dpg.start_dearpygui()

cnt = 0
while dpg.is_dearpygui_running():

    cnt += 1
    if cnt == 5:
        read_new_data()
        cnt = 0
    dpg.render_dearpygui_frame()
interval = 0  # stop timer
dpg.destroy_context()

with open("out.bin", "rb") as f_in:
    with open("out.h", "w+") as f_out:
        f_out.write("{\n")
        for line in f_in.readlines():
            for byte in line:
                f_out.write(hex(byte))
                f_out.write(",")
        f_out.write("\n};")

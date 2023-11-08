
# 用于修改仓库所在路径和CUBE项目的版本

import os

for root, dirs, files in os.walk("./"):

    for file in files:

        if file.endswith(".uvprojx"):
            ctx = ""
            filepath = os.path.join(root, file)
            # 路径更新
            with open(filepath, "r") as f:
                ctx = f.read()
            with open(filepath, "w") as f:
                ctx = ctx.replace("C:/Users/Administrator/STM32Cube/Repository",
                                  "C:/Users/uYanki/STM32Cube/Repository")
                f.write(ctx)

        if file.endswith(".ioc"):
            ctx = ""
            filepath = os.path.join(root, file)
            # 版本迁移
            with open(filepath, "r") as f:
                ctx = f.read()
            with open(filepath, "w") as f:
                ctx = ctx.replace("MxCube.Version=6.7.0",
                                  "MxCube.Version=6.5.0")
                f.write(ctx)

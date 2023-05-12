import os
import shutil


IGNORED_FILES = []
IGNORED_DIRS = []


def conatins_chinese(str):
    for ch in str:
        if u'\u4e00' <= ch <= u'\u9fff':
            return True
    return False


for root, dirs, files in os.walk('.'):
    for file in files:
        if file.endswith('.pdf'):
            if file not in IGNORED_FILES:
                if conatins_chinese(file):
                    continue
                newname = file.lower().replace(' ', '_')
                for i in range(5):
                    newname = newname.replace('__', '_')
                os.rename(
                    os.path.join(root, file),
                    os.path.join(root, newname)
                )

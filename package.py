import os
import tarfile
import re
from jenkinshelper import run

with open("src/constants.hpp") as constants_file:
    VERSION = re.search(r'programVersion = "v([\d\.]*)"', constants_file.read()).group(1)

assert VERSION != ""

run("scons")
with tarfile.open("JNTETRI {}.tar.xz".format(VERSION), "w:xz") as tar:
    tar.add("bin/jntetri")
    tar.add("data")
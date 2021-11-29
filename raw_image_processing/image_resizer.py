"""
resize image to 1056x1056 resolution
"""

import os, sys
import cv2
import numpy as np


def get_key(x):
    x = x[:-4].split("_")[-1]
    return int(x)


if __name__ == "__main__":
    RESOLUTION = (1056, 1056)
    in_dir = "./data/episode_1/pi_3_frames"
    out_dir = "{}_1056".format(in_dir)

    filenames = os.listdir(in_dir)
    filenames = sorted(filenames, key=lambda x: get_key(x))

    for name in filenames:
        print(name)
        img = cv2.imread("{}/{}".format(in_dir, name))
        assert img is not None
        img = cv2.resize(img, (1056, 1056), cv2.INTER_AREA)

        cv2.imwrite("{}/{}".format(out_dir, name), img)
        # sys.exit(-1)

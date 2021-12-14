"""
convert json file (ground truth manually annotated) to yolo format
"""

import json
import os


def get_key(x):
    x = x[:-5].split("_")[1]
    return int(x)


if __name__ == "__main__":
    IN_DIR = r"./data/episode_1/ground_truth/gt_json/"
    OUT_DIR = r"./data/episode_1/ground_truth/gt_yolo/"

    filenames = os.listdir(IN_DIR)
    filenames = sorted(filenames, key=lambda x: get_key(x))

    for name in filenames:
        with open("{}/{}".format(IN_DIR, name)) as json_input:
            j_obj = json.load(json_input)
        f_num = name[:-5].split("_")[1]
        j_obj = j_obj["{}".format(f_num)]

        cam_objects = [j_obj["cam_1"], j_obj["cam_2"], j_obj["cam_3"]]

        for cam in range(1, 4):
            out_filename = "frame_{}_{}.txt".format(cam, f_num)
            with open("{}/{}".format(OUT_DIR, out_filename), 'w') as out:
                objects = cam_objects[cam - 1]
                for obj in objects:
                    x, y, w, h = obj["coords"]
                    print(x)

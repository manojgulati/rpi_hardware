import os


def get_key(x):
    key = int(x.split(".jpg")[0].split("_")[2])
    return key


if __name__ == "__main__":
    DATA_DIR = "data/episode_1/pi_1_frames/"
    OUTPUT_FILE = "pi_1_test.txt"
    filenames = sorted(os.listdir(DATA_DIR), key=lambda x: get_key(x))

    with open(OUTPUT_FILE, 'w') as out_file:
        for name in filenames:
            out_file.write("{}\n".format(name))

"""
module to extract synchronized images from all 3 Rpi boards. Uses Image_Processor module
"""
import cv2
import sys
import numpy as np
import math

# global variables
FPS = 8
FRAME_NUMBER = 0
TIME_DELTA = 20  # acceptable time drift (ms) between cameras
INTER_FRAME_DELAY = 125  # ms for 8 FPS
video_frame_pointer_1 = 0  # pointer to specific frame number in video of pi 1 cam
video_frame_pointer_2 = 0  # pointer to specific frame number in video of pi 2 cam
video_frame_pointer_3 = 0  # pointer to specific frame number in video of pi 3 cam
EPISODE = 2

# parameters used for image enhancements -- by process_frame_v2 method
gamma = 1.1
inBlack = np.array([10, 10, 10], dtype=np.float32)
inWhite = np.array([190, 255, 190], dtype=np.float32)
inGamma = np.array([gamma, gamma, gamma], dtype=np.float32)
outBlack = np.array([0, 0, 0], dtype=np.float32)
outWhite = np.array([500, 500, 500], dtype=np.float32)


def white_balance(img, amp=1.3):
    result = cv2.cvtColor(img, cv2.COLOR_BGR2LAB)
    avg_a = np.average(result[:, :, 1])
    avg_b = np.average(result[:, :, 2])
    result[:, :, 1] = result[:, :, 1] - ((avg_a - 128) * (result[:, :, 0] / 255.0) * amp)
    result[:, :, 2] = result[:, :, 2] - ((avg_b - 128) * (result[:, :, 0] / 255.0) * amp)
    result = cv2.cvtColor(result, cv2.COLOR_LAB2BGR)
    return result


def adjust_gamma(image, gamma=1.0):
    # build a lookup table mapping the pixel values [0, 255] to
    # their adjusted gamma values
    invGamma = 1.0 / gamma
    table = np.array([((i / 255.0) ** invGamma) * 255
                      for i in np.arange(0, 256)]).astype("uint8")
    # apply gamma correction using the lookup table
    return cv2.LUT(image, table)


def adjust_blacklevel(image, gamma=1.2):
    invGamma = 1.0 / gamma
    table = np.array([((i / 255.0) ** invGamma) * 255 for i in np.arange(0, 256)]).astype("uint8")
    return cv2.LUT(image, table)


def adjust_contrast_brightness(img, contrast=1.8, brightness=20):
    img = cv2.convertScaleAbs(img, alpha=contrast, beta=brightness)
    return img


def process_frame_v2(img):
    img = cv2.rotate(img, cv2.ROTATE_180)
    img = np.clip((img - inBlack) / (inWhite - inBlack), 0, 255)
    img = (img ** (1 / inGamma)) * (outWhite - outBlack) + outBlack
    img = np.clip(img, 0, 255).astype(np.uint8)
    img = cv2.fastNlMeansDenoisingColored(img, None, 6, hColor=7, templateWindowSize=5, searchWindowSize=3)

    # resize frame to 1056x1056
    img = cv2.resize(img, (1056,1056), cv2.INTER_AREA)
    return img


def process_frame(img):
    # process the image for quality enhancement
    img = cv2.rotate(img, cv2.ROTATE_180)
    img = white_balance(img, amp=2)
    img = adjust_contrast_brightness(img, contrast=1.5, brightness=15)
    img = cv2.bilateralFilter(img, 6, 21, 11)
    return img


def save_frame(frame, cam_id, frame_num):
    OUT_DIR = "./data/episode_{}/pi_{}_frames_1056_v2/".format(EPISODE, cam_id)
    # print(OUT_DIR)
    frame_name = "frame_{}_{}.jpg".format(cam_id, frame_num)
    cv2.imwrite("{}/{}".format(OUT_DIR, frame_name), frame)


def extract_frames(video_frame_pointer_1, video_frame_pointer_2, video_frame_pointer_3):
    pi_1_video.set(cv2.CAP_PROP_POS_FRAMES, video_frame_pointer_1)
    pi_2_video.set(cv2.CAP_PROP_POS_FRAMES, video_frame_pointer_2)
    pi_3_video.set(cv2.CAP_PROP_POS_FRAMES, video_frame_pointer_3)

    ret, frame = pi_1_video.read()
    # frame = process_frame(frame)
    frame = process_frame_v2(frame)
    save_frame(frame, 1, video_frame_pointer_2)

    ret, frame = pi_2_video.read()
    # frame = process_frame(frame)
    frame = process_frame_v2(frame)
    save_frame(frame, 2, video_frame_pointer_2)

    ret, frame = pi_3_video.read()
    # frame = process_frame(frame)
    frame = process_frame_v2(frame)
    save_frame(frame, 3, video_frame_pointer_2)


def extract_nearest_frame(timestamp_2, index, timestamp_1, timestamp_3):
    global FRAME_NUMBER, video_frame_pointer_1, video_frame_pointer_2, video_frame_pointer_3

    # timestamp_1 = pi_1_logs[video_frame_pointer_1]
    # timestamp_3 = pi_3_logs[video_frame_pointer_3]

    if abs(timestamp_2 - timestamp_1) <= TIME_DELTA and abs(timestamp_2 - timestamp_3) <= TIME_DELTA:  # 20 ms apart
        print("Extracting Frames..")
        # extract and save frames from all 3 cameras
        extract_frames(video_frame_pointer_1, video_frame_pointer_2, video_frame_pointer_3)
        # FRAME_NUMBER += 1
        video_frame_pointer_1 += 1
        video_frame_pointer_2 += 1
        video_frame_pointer_3 += 1
    else:
        # print("Frames not in sync..\n")
        while True:
            if abs(timestamp_2 - timestamp_1) >= TIME_DELTA:
                frames_to_drop = math.ceil((abs(timestamp_2 - timestamp_1) - TIME_DELTA) / INTER_FRAME_DELAY)
                if timestamp_2 - timestamp_1 > 0:
                    # cam2 is ahead of cam 1 -- drop cam 1's frames
                    video_frame_pointer_1 += frames_to_drop
                else:
                    video_frame_pointer_2 += frames_to_drop
                    index += frames_to_drop
                    timestamp_2 = pi_2_logs[video_frame_pointer_2]

            if abs(timestamp_2 - timestamp_3) >= TIME_DELTA:
                frames_to_drop = math.ceil((abs(timestamp_2 - timestamp_3) - TIME_DELTA) / INTER_FRAME_DELAY)
                if timestamp_2 - timestamp_3 > 0:
                    video_frame_pointer_3 += frames_to_drop
                else:
                    video_frame_pointer_2 += frames_to_drop
                    video_frame_pointer_1 += frames_to_drop  # because 2 and 1 already in sync before this block

            timestamp_1 = pi_1_logs[video_frame_pointer_1]
            timestamp_2 = pi_2_logs[video_frame_pointer_2]
            timestamp_3 = pi_3_logs[video_frame_pointer_3]
            if abs(timestamp_2 - timestamp_1) <= TIME_DELTA and abs(timestamp_2 - timestamp_3) <= TIME_DELTA:
                extract_nearest_frame(timestamp_2, index, timestamp_1, timestamp_3)
                break
    return index


if __name__ == "__main__":
    PI_1_VIDEO_NAME = "./data/episode_{}/pi_1.mp4".format(EPISODE)
    PI_1_LOGS_NAME = "./data/episode_{}/pi_1_logs.txt".format(EPISODE)

    PI_2_VIDEO_NAME = "./data/episode_{}/pi_2.mp4".format(EPISODE)
    PI_2_LOGS_NAME = "./data/episode_{}/pi_2_logs.txt".format(EPISODE)

    PI_3_VIDEO_NAME = "./data/episode_{}/pi_3.mp4".format(EPISODE)
    PI_3_LOGS_NAME = "./data/episode_{}/pi_3_logs.txt".format(EPISODE)

    # open image and log files
    pi_1_video = cv2.VideoCapture(PI_1_VIDEO_NAME)
    assert pi_1_video is not None
    pi_2_video = cv2.VideoCapture(PI_2_VIDEO_NAME)
    assert pi_2_video is not None
    pi_3_video = cv2.VideoCapture(PI_3_VIDEO_NAME)
    assert pi_3_video is not None

    pi_1_logs = ""
    pi_2_logs = ""
    pi_3_logs = ""

    # read log files
    with open(PI_1_LOGS_NAME) as pi_1_logs:
        with open(PI_2_LOGS_NAME) as pi_2_logs:
            with open(PI_3_LOGS_NAME) as pi_3_logs:
                pi_1_logs = pi_1_logs.read().split("\n")
                pi_1_logs = [int(i) for i in pi_1_logs]
                pi_2_logs = pi_2_logs.read().split("\n")
                pi_2_logs = [int(i) for i in pi_2_logs]
                pi_3_logs = pi_3_logs.read().split("\n")
                pi_3_logs = [int(i) for i in pi_3_logs]

    # extract frames by taking pi 2 as reference
    pi_2_logs_len = len(pi_2_logs)
    index = 0
    while index < pi_2_logs_len:
        timestamp_1 = pi_1_logs[video_frame_pointer_1]
        timestamp_2 = pi_2_logs[video_frame_pointer_2]
        timestamp_3 = pi_3_logs[video_frame_pointer_3]
        print("\nFrame Index - {}".format(index))
        index = extract_nearest_frame(timestamp_2, index, timestamp_1, timestamp_3)
        index += 1
        print(video_frame_pointer_1, video_frame_pointer_2, video_frame_pointer_3)
        # index += increment

        # break

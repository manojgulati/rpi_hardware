"""
module to process raw images
"""
import numpy as np
import cv2
import sys
import matplotlib.pyplot as plt
# import rawpy
import imageio


def remap(v, min, max):
    return (v - min) * 255.0 / (max - min)


def cs_get_rgb(r, g, b, min, max):
    r = remap(r, min, max)
    g = remap(g, min, max)
    b = remap(b, min, max)
    return r, g, b


def contrast_stretch(img):
    min = np.min(img)
    max = np.max(img)
    rows, cols, _ = img.shape
    for row, col in np.ndindex((rows, cols)):
        r, g, b = img[row][col]
        img[row, col] = cs_get_rgb(r, g, b, min, max)
    return img


def adjust_gamma(image, gamma=1.0):
    # build a lookup table mapping the pixel values [0, 255] to
    # their adjusted gamma values
    invGamma = 1.0 / gamma
    table = np.array([((i / 255.0) ** invGamma) * 255
                      for i in np.arange(0, 256)]).astype("uint8")
    # apply gamma correction using the lookup table
    return cv2.LUT(image, table)


def white_balance(img, amp=1.1):
    result = cv2.cvtColor(img, cv2.COLOR_BGR2LAB)
    avg_a = np.average(result[:, :, 1])
    avg_b = np.average(result[:, :, 2])
    result[:, :, 1] = result[:, :, 1] - ((avg_a - 128) * (result[:, :, 0] / 255.0) * amp)
    result[:, :, 2] = result[:, :, 2] - ((avg_b - 128) * (result[:, :, 0] / 255.0) * amp)
    result = cv2.cvtColor(result, cv2.COLOR_LAB2BGR)
    return result


def adjust_contrast_brightness(img, contrast=1.8, brightness=20):
    img = cv2.convertScaleAbs(img, alpha=contrast, beta=brightness)
    return img


def adjust_blacklevel(image, gamma=1.2):
    invGamma = 1.0 / gamma
    table = np.array([((i / 255.0) ** invGamma) * 255 for i in np.arange(0, 256)]).astype("uint8")
    return cv2.LUT(image, table)


def vithurson(frame, gamma):
    inBlack = np.array([10, 10, 10], dtype=np.float32)
    inWhite = np.array([190, 255, 190], dtype=np.float32)
    inGamma = np.array([gamma, gamma, gamma], dtype=np.float32)
    outBlack = np.array([0, 0, 0], dtype=np.float32)
    outWhite = np.array([500, 500, 500], dtype=np.float32)
    # Display the resulting frame
    img = np.clip((frame - inBlack) / (inWhite - inBlack), 0, 255)
    img = (img ** (1 / inGamma)) * (outWhite - outBlack) + outBlack
    img = np.clip(img, 0, 255).astype(np.uint8)
    (height, width, _) = img.shape
    scale = int(1080 / height)
    #        img=cv2.medianBlur(img,5)
    # img = cv2.fastNlMeansDenoisingColored(img, None, 7, 7, 5, 21)
    return img


if __name__ == "__main__":
    video_file_path = "data/episode_2/pi_2.avi"

    video = cv2.VideoCapture(video_file_path)
    assert video is not None

    ret, frame = video.read()
    while (ret):
        print()
        if not ret:
            print("can't read video, exiting!!\n")
            sys.exit(-1)

        frame = cv2.rotate(frame, cv2.ROTATE_180)
        cv2.imshow("Original", frame)

        # process image
        img = frame

        cv2.imshow("vithurson", vithurson(img, 1.1))

        gamma = 1.1
        inBlack = np.array([10, 10, 10], dtype=np.float32)
        inWhite = np.array([190, 255, 190], dtype=np.float32)
        inGamma = np.array([gamma, gamma, gamma], dtype=np.float32)
        outBlack = np.array([0, 0, 0], dtype=np.float32)
        outWhite = np.array([500, 500, 500], dtype=np.float32)
        img = np.clip((frame - inBlack) / (inWhite - inBlack), 0, 255)
        img = (img ** (1 / inGamma)) * (outWhite - outBlack) + outBlack
        img = np.clip(img, 0, 255).astype(np.uint8)

        # img = white_balance(img, amp=2)
        # cv2.imshow("white_balanced", img)

        # img = adjust_contrast_brightness(img, contrast=1.5, brightness=15)
        # cv2.imshow("contrasted", img)

        # img = adjust_blacklevel(img, gamma=1)
        # cv2.imshow("black_level", img)

        # img = adjust_gamma(img, gamma=1.5)
        # cv2.imshow("gamma", img)

        # img = cv2.GaussianBlur(img, ksize=(5, 5), sigmaX=0, sigmaY=0)
        # img = cv2.medianBlur(img, ksize=5)
        img = cv2.bilateralFilter(img, 11, 45, 45)
        # img = contrast_stretch(img)
        cv2.imshow("Blur", img)

        # img = cv2.fastNlMeansDenoisingColored(img, None, 7, 7, 2, 10)
        # cv2.imshow("Denoised", img)

        cv2.waitKey(100)
        ret, frame = video.read()

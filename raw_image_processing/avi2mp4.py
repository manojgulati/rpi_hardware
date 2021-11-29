"""
module to convert avi to mp4. Avi format doesn't support frame seek operation
souce -- https://ottverse.com/ffmpeg-convert-avi-to-mp4-lossless/
"""

import moviepy.editor as editor
import subprocess
import os

input_video_name = "./data/episode_2/pi_1.avi"
output_video_name = "{}.mp4".format(input_video_name[:-4])
# video_clip = editor.VideoFileClip(input_video_name)
# video_clip.write_videofile(output_video_name)

# subprocess.run(["ffmpeg" -i input_filename.avi -c:v copy -c:a copy -y output_filename.mp4])
os.system("ffmpeg -i {} -c:v copy -c:a copy -y {}".format(input_video_name, output_video_name))

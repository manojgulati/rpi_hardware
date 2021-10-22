from os import system as s
share_regions = [0,25,49,53,75,90]
for share_region in share_regions:
    s("./th_video "+str(share_region)+" 100")

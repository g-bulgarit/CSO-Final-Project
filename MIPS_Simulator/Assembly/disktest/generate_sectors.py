sector_data = ["0", "1", "2", "3", "4", "5", "6", "7"]
sectors = 512
words_in_sector = 4

with open("diskin.txt", "w+") as wfp:
    lines = []
    for dat in sector_data:
        val = "0000000" + dat + "\n"
        for _ in range((sectors // words_in_sector)):
            wfp.write(val)
            

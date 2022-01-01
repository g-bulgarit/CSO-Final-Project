sector_data = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
bytes_in_sector = 512
bytes_in_word = 4

with open("diskin.txt", "w+") as wfp:
    lines = []
    for dat in sector_data:
        val = "0000000" + dat + "\n"
        for _ in range((bytes_in_sector // bytes_in_word)):
            wfp.write(val)
            

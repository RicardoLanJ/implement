from PIL import Image
from numpy import * 
from bisect import bisect_left

def quantize(original, level):
	im = array(original)
	seg = round(256 / (level - 1))

	levels = [0]
	tempLevel = 0;
	for i in range(1, level - 1): # get the sequence of pix that will exist in new gray image
		tempLevel += seg
		levels.append(tempLevel)
	levels.append(255)

	for index, tlevel in ndenumerate(im):
		pos = bisect_left(levels, tlevel)
		if tlevel > levels[pos] - (seg / 2) : # [0--4---8] ,seg=4 ==> [0-2 2-6 6-8]
			im[index] = levels[pos]
		else:
			im[index] = levels[pos - 1] if pos > 0 else 0
	return Image.fromarray(im)

if __name__  == '__main__':
	original = Image.open("61.png")
	levels = [128, 32, 8, 4, 2]
	for level in levels:
		result = quantize(original, level)
		result.show()
		#result.save(str(level) + ".png") #if you want to save
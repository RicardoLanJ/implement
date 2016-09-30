from PIL import Image
from numpy import * 

def scale(original, newSize):
	im = array(original)
	ox, oy = im.shape
	sfx, sfy = im.shape[0] / newSize[0], im.shape[1] / newSize[1] #scale factor

	nim = zeros(newSize, dtype=uint8)

	for index, level in ndenumerate(nim):
		 xy = [index[0] * sfx, index[1] * sfy]
		 i, j = [floor(it) for it in xy]
		 u, v = [it % 1 for it in xy]
		 
		 #f(x,y) = f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1) 
		 nim[index] =  round( ((1-u)*(1-v)*im[i][j] if i < ox and j < oy else 0) 
		 				+ ((1-u)*v*im[i][j+1] if i < ox and j+1 < oy else 0)
	   					+ (u*(1-v)*im[i+1][j] if i+1 < ox and j < oy else 0)
		 				+ (u*v*im[i+1][j+1]   if i+1 < ox and j+1 < oy else 0))
	return Image.fromarray(nim)

if __name__  == '__main__':
	original = Image.open("61.png")
	nsize = [(128,192), (200,300), (300,450), (200,500), (64, 96), (32, 48), (16, 24), (8, 12)]

	for newSize in nsize:
		result = scale(original, newSize)
		result.show()
		#result.save(str(newSize[1]) + ".png") #if you want to save

#i though it is so clear to have few comment
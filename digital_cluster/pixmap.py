from PIL import Image
import numpy as np

def convert():
    blue_rgb = (0, 60, 100)
    tolerance = 50
    # Open the image and convert to RGBA (handles transparency)
    img = Image.open("./fonts_icon/car.png").convert('RGBA')
    data = np.array(img)
    
    # Define white and near-white colors
    r, g, b, a = data[:,:,0], data[:,:,1], data[:,:,2], data[:,:,3]
    
    # Create mask for white pixels (with tolerance)
    white_mask = (
        (r > 255 - tolerance) & 
        (g > 255 - tolerance) & 
        (b > 255 - tolerance)
    )
    
    # Change white pixels to blue (preserving alpha)
    data[:,:,:3][white_mask] = blue_rgb
    
    # Create new image from modified data
    new_img = Image.fromarray(data, 'RGBA')
    new_img.save("./fonts_icon/car2.png")


convert()
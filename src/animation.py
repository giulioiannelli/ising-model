"""
Matplotlib Animation Example

author: Jake Vanderplas
email: vanderplas@astro.washington.edu
website: http://jakevdp.github.com
license: BSD
Please feel free to use and modify this, but keep the above information. Thanks!
"""

import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
from matplotlib.colors import ListedColormap


cmap = ListedColormap(["k", "w"])

# First set up the figure, the axis, and the plot element we want to animate
fig, ax = plt.subplots()

Lx = 1000
Ly = 1000
N = Lx*Ly
T = 2.3

a=[np.fromfile(f"res/N={N}/cfg_Lx={Lx:d}_Ly={Ly:d}_T={T:.2f}_t={t:d}.bin", dtype=np.int8).reshape(Lx, Ly)
    for t in range(0, 10000, 100)]
im=plt.imshow(a[0],interpolation='none', cmap=cmap)
# initialization function: plot the background of each frame
def init():
    im.set_data(a[0])
    return [im]

# animation function.  This is called sequentially
def animate(i):
    tmp = a[i]    
    im.set_array(tmp)
    return [im]

# call the animator.  blit=True means only re-draw the parts that have changed.
anim = animation.FuncAnimation(fig, animate, init_func=init,
                               frames=100, interval=60, blit=True)

# save the animation as an mp4.  This requires ffmpeg or mencoder to be
# installed.  The extra_args ensure that the x264 codec is used, so that
# the video can be embedded in html5.  You may need to adjust this for
# your system: for more information, see
# http://matplotlib.sourceforge.net/api/animation_api.html
anim.save('basic_animation.mp4', fps=60, extra_args=['-vcodec', 'libx264'])

plt.show()
#Let's step through this and see what's going on. After importing required pieces of numpy and matplotlib, The script sets up the plot:

# fig = plt.figure()
# ax = plt.axes(xlim=(0, 2), ylim=(-2, 2))
# line, = ax.plot([], [], lw=2)
# #Here we create a figure window, create a single axis in the figure, and then create our line object which will be modified in the animation. Note that here we simply plot an empty line: we'll add data to the line later.

# #Next we'll create the functions which make the animation happen. init() is the function which will be called to create the base frame upon which the animation takes place. Here we use just a simple function which sets the line data to nothing. It is important that this function return the line object, because this tells the animator which objects on the plot to update after each frame:

# def init():
#     line.set_data([], [])
#     return line,
# #The next piece is the animation function. It takes a single parameter, the frame number i, and draws a sine wave with a shift that depends on i:

# # animation function.  This is called sequentially
# def animate(i):
#     x = np.linspace(0, 2, 1000)
#     y = np.sin(2 * np.pi * (x - 0.01 * i))
#     line.set_data(x, y)
#     return line,
# #Note that again here we return a tuple of the plot objects which have been modified. This tells the animation framework what parts of the plot should be animated.

# #Finally, we create the animation object:

# anim = animation.FuncAnimation(fig, animate, init_func=init,
#                                frames=100, interval=20, blit=True)
#This object needs to persist, so it must be assigned to a variable. We've chosen a 100 frame animation with a 20ms delay between frames. The blit keyword is an important one: this tells the animation to only re-draw the pieces of the plot which have changed. The time saved with blit=True means that the animations display much more quickly.

#We end with an optional save command, and then a show command to show the result. Here's what the script generates:

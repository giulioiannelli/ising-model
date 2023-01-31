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

pathtoconfig = "res/config_files/.config_100x100_test_savemany"
with open(pathtoconfig) as f:
    lines = f.readlines()

tMCMC, L1, L2, beta_m, beta_M, beta_stp, MODE_save = int(lines[0]),\
    int(lines[1]), int(lines[2]), float(lines[3]), float(lines[4]),\
    float(lines[5]), int(lines[-1])
MODE_init = lines[6]
MODE_upd = lines[7]
print("tMCMC: ", tMCMC, "\nL1: ", L1, "\nL2: ", L2, "\nbeta_m: ", beta_m, 
    "\nbeta_M: ", beta_M, "\nbeta_stp: ", beta_stp, 
    "\nMODE_init: ", MODE_init[:-1], "\nMODE_upd: ", MODE_upd[:-1], "\nMODE_save: ", MODE_save)

sizepath = f"N={L1*L2:d}" if L1 == L2 else f"L1={L1:d}_L2={L2:d}"
betaval = np.arange(beta_m, beta_M, beta_stp)
if MODE_save == tMCMC:
    tlist = [0, tMCMC]
else:
    tlist = [x for x in range(MODE_save, tMCMC+1, tMCMC//MODE_save)]
pathtoconfig = [f"res/varbeta_isingcfg/{sizepath}/beta={b:.3g}/SCONF_t={t:d}.bin"
    for b in betaval for t in tlist]

config = [np.fromfile(pth, dtype=np.int8).reshape(L1, L2) for pth in pathtoconfig]


im=plt.imshow(config[0],interpolation='none', cmap=cmap)
# initialization function: plot the background of each frame
def init():
    im.set_data(config[0])
    return [im]

# animation function.  This is called sequentially
def animate(i):
    tmp = config[i]    
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

import numpy as np


"""
Script that counts the number of uncovered pairs

Runs in about 2 second 

current number of uncovered pairs : 2048147
"""
# Initialization of the matrix
N = 5721 
A = np.zeros((N, N))
p, q = np.indices(A.shape)

# Whenever p or q < 3 then we already know everything
mask1 = ((p < 3) | (q < 3))


# Bounds from Theorem 16 :
mask_thm_16 = ((np.minimum(p, q) >= 331) & (np.maximum(p, q) >= 364))

# Bounds from Proposition 19 :
mask_prop_19 =  ((((p==13)|(p==17)|(p==18)|(p==19)|(p>=23)) & (q >= 19*p)) |
                (((q==13)|(q==17)|(q==18)|(q==19)|(q>=23)) & (p >= 19*q)))

# Bounds from Proposition 21 :
mask_prop_21 = ((((p==3) & (q>=1080))|((p==4) & (q>=1360))|((p==5) & (q>=1300))|((p==7) & (q>=1820))|((p==8) & (q>=2080))|((p==9) & (q>=2340))|((p==10) & (q>=2600))|((p==11) & (q>=2860))|((p==12) & (q>=3120))|((p==14) & (q>=3640))|((p==15) & (q>=3900))|((p==16) & (q>=4160))|((p==20) & (q>=5200))|((p==21) & (q>=5460))|((p==22) & (q>=5720))) | 
                (((q==3) & (p>=1080))|((q==4) & (p>=1360))|((q==5) & (p>=1300))|((q==7) & (p>=1820))|((q==8) & (p>=2080))|((q==9) & (p>=2340))|((q==10) & (p>=2600))|((q==11) & (p>=2860))|((q==12) & (p>=3120))|((q==14) & (p>=3640))|((q==15) & (p>=3900))|((q==16) & (p>=4160))|((q==20) & (p>=5200))|((q==21) & (p>=5460))|((q==22) & (p>=5720))))

# Bounds from Proposition 26 :
mask_prop_26 = (((p == 6) & q >= 342) |
                ((q == 6) & p >= 342))



# Create coordinate grids

mask = (mask1 | mask_thm_16 | mask_prop_19 | mask_prop_21 | mask_prop_26)
print(np.sum(~mask))

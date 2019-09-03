
import sys

if len(sys.argv) != 2:
    print("Usage: python3 black-body-radiation.py <temperature>")
    exit()


e = 2.7182818284590452353602874713527
h = 6.62607004e-34  # Planck's constant [m^2 kg / s]
c = 299792458  # Speed of light in a vacuum [m / s]
k = 1.38064852e-23  # Boltzmann constant [m^2 kg s^-2 K^-1]


def b(t, w):
    return 2 * h * c ** 2 / (w ** 5 * (e ** (h * c / (w * k * t)) - 1))


T = int(sys.argv[1])

print("{{{}}}".format(", ".join(str(b(T, w * 1e-9)) for w in range(380, 780 + 5, 5))))

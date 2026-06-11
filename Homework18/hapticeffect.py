import numpy as np
import matplotlib.pyplot as plt

# Parameters
theta = np.linspace(-2, 2, 500)
theta0 = 0.0   # center (rad)
sigma  = 0.25  # width (rad)
A      = 0.80  # amplitude

# Bump
bump = A * np.exp(-(theta - theta0)**2 / (2 * sigma**2))

# Dip
dip = -A * np.exp(-(theta - theta0)**2 / (2 * sigma**2))

# Toggle switch
toggle = -A * ((theta - theta0) / sigma**2) * np.exp(-(theta - theta0)**2 / (2 * sigma**2))
toggle = toggle / np.max(np.abs(toggle))

# Plot
fig, axes = plt.subplots(1, 3, figsize=(13, 4), sharey=True)
fig.suptitle('Haptic effects — normalized force vs displacement', fontsize=13)

configs = [
    (axes[0], bump,   '#1D9E75', 'Bump'),
    (axes[1], dip,    '#378ADD', 'Dip'),
    (axes[2], toggle, '#D85A30', 'Toggle switch'),
]

for ax, F, color, title in configs:
    ax.plot(theta, F, color=color, linewidth=2.5)
    ax.fill_between(theta, F, 0, alpha=0.12, color=color)
    ax.axhline(0,      color='gray', linewidth=0.8, linestyle='--')
    ax.axvline(theta0, color='gray', linewidth=0.8, linestyle=':')
    ax.set_title(title, fontsize=12)
    ax.set_xlabel('θ — paddle angle (rad)')
    ax.set_xlim(-2, 2)
    ax.set_ylim(-1.1, 1.1)
    ax.set_yticks([-1, -0.5, 0, 0.5, 1])
    ax.grid(True, alpha=0.25)

axes[0].set_ylabel('Normalized force output')

# Equations
axes[0].text(-1.9, -0.95, r'$F = A \cdot e^{-(\theta-\theta_0)^2/2\sigma^2}$',
             fontsize=9, color='#1D9E75')
axes[1].text(-1.9, -0.95, r'$F = -A \cdot e^{-(\theta-\theta_0)^2/2\sigma^2}$',
             fontsize=9, color='#378ADD')
axes[2].text(-1.9, -0.95, r'$F = -A\frac{(\theta-\theta_0)}{\sigma^2} e^{-(\theta-\theta_0)^2/2\sigma^2}$',
             fontsize=9, color='#D85A30')

plt.tight_layout()
plt.savefig('haptic_effects.png', dpi=150)
plt.show()
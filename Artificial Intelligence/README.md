# Artificial Intelligence Lab

Lab work, assignments and practice code for the AI course, mostly built with TensorFlow/Keras.

## Folder Contents

| Folder | What's inside |
|---|---|
| `Neural_Network_Basics/` | Polynomial regression with a DNN, a CNN on MNIST with early stopping, and a Fashion-MNIST DNN with BatchNorm (plus its CSV) |
| `Image_Classification/` | Leaf classification with a custom CNN and with MobileNetV2, and 3-class CIFAR-10 with VGG16 |
| `Assignment Solve/` | Assignment 1 to 3 solutions (polynomial regression, MNIST custom CNN, MNIST odd digits) |
| `Lab_Practice_Old/` | Older lab practice (FCNN vs CNN, MobileNet transfer learning, A* search and 8-puzzle, Pima diabetes classifier, OpenCV grayscale) |
| `My Lab 2026/` | 2026 lab practice scripts (regression, MNIST even/odd, MobileNet and VGG16 transfer learning), lab question images and past solutions PDF |

The leaf classification notebooks and the Fashion-MNIST notebook were written for Google Colab and load their data from Google Drive.

## Setup

Create a virtual environment.

```bash
python -m venv venv        # Windows
python3 -m venv venv       # Linux / macOS
```

Activate it.

```bash
venv\Scripts\activate          # Windows (CMD / PowerShell)
source venv/Scripts/activate   # Windows (Git Bash)
source venv/bin/activate       # Linux / macOS
```

Install the packages.

```bash
python -m pip install --upgrade pip
python -m pip install numpy matplotlib
python -m pip install tensorflow
python -m pip install notebook
python -m pip install scikit-learn
python -m pip install pandas opencv-python   # needed by a few notebooks
```

Check the install.

```bash
which python   # in Git Bash, should point inside venv
python -c "import tensorflow as tf; print('TensorFlow version:', tf.__version__)"
```

Run these after activating the virtual environment. Once it is active, you can use it from any other directory too.

Start Jupyter to open the notebooks.

```bash
jupyter notebook
```

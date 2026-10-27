import tensorflow as tf
import os
os.environ["TF_ENABLE_ONEDNN_OPTS"] = "0"

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Flatten, Conv2D, MaxPooling2D, Dropout
from tensorflow.keras.datasets import cifar10
from tensorflow.keras.utils import to_categorical
from tensorflow.keras.layers import Input, Flatten, Dense

# Load and preprocess CIFAR-10 dataset
(x_train, y_train), (x_test, y_test) = cifar10.load_data()

# Normalize the image data to range [0, 1]
x_train = x_train.astype('float16') / 255.0
x_test = x_test.astype('float16') / 255.0


# One-hot encode the labels
y_train = to_categorical(y_train, 10)
y_test = to_categorical(y_test, 10)


model = Sequential([
    Input(shape=(32, 32, 3)),  # Example for image input
    Flatten(),                 # Flatten image to vector
    Dense(64, activation='relu'),
    Dense(32, activation='relu'),
    Dense(1, activation='sigmoid')  # Binary classification
])

# Define the Fully Connected Neural Network (FCNN)
def build_fcnn():
    model = Sequential([
        Flatten(input_shape=(32, 32, 3)),  # Flatten the input images
        Dense(512, activation='relu'),
        Dropout(0.5),  # Regularization to prevent overfitting
        Dense(256, activation='relu'),
        Dropout(0.5),
        Dense(10, activation='softmax')  # Output layer for 10 classes
    ])
    model.compile(optimizer='adam', 
                  loss='categorical_crossentropy', 
                  metrics=['accuracy'])
    return model

# Define the Convolutional Neural Network (CNN)
def build_cnn():
    model = Sequential([
        Conv2D(32, (3, 3), activation='relu', input_shape=(32, 32, 3)),
        MaxPooling2D((2, 2)),
        Dropout(0.25),

        Conv2D(64, (3, 3), activation='relu'),
        MaxPooling2D((2, 2)),
        Dropout(0.25),

        Conv2D(128, (3, 3), activation='relu'),
        Flatten(),  # Flatten the output before feeding into Dense layers

        Dense(256, activation='relu'),
        Dropout(0.5),
        Dense(10, activation='softmax')  # Output layer for 10 classes
    ])
    model.compile(optimizer='adam', 
                  loss='categorical_crossentropy', 
                  metrics=['accuracy'])
    return model

# Instantiate and train the Fully Connected Neural Network
print("Training Fully Connected Neural Network (FCNN)...")
fcnn_model = build_fcnn()
fcnn_model.fit(x_train, y_train, epochs=10, batch_size=64, validation_split=0.2)
fcnn_loss, fcnn_accuracy = fcnn_model.evaluate(x_test, y_test, verbose=0)
print(f"FCNN Test Accuracy: {fcnn_accuracy * 100:.2f}%")

# Instantiate and train the Convolutional Neural Network
print("Training Convolutional Neural Network (CNN)...")
cnn_model = build_cnn()
cnn_model.fit(x_train, y_train, epochs=10, batch_size=64, validation_split=0.2)
cnn_loss, cnn_accuracy = cnn_model.evaluate(x_test, y_test, verbose=0)
print(f"CNN Test Accuracy: {cnn_accuracy * 100:.2f}%")

import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Flatten, Dropout
from tensorflow.keras.datasets import mnist
from tensorflow.keras.utils import to_categorical

# Load and preprocess the dataset
(x_train, y_train), (x_test, y_test) = mnist.load_data()

# Normalize the dataset (values between 0 and 1)
x_train = x_train.astype('float32') / 255.0
x_test = x_test.astype('float32') / 255.0

# Flatten the input images (28x28 -> 784)
input_shape = (28, 28)

# One-hot encode the labels
y_train = to_categorical(y_train, 10)
y_test = to_categorical(y_test, 10)

# Build the Fully Connected Neural Network (FCNN)
def build_fcnn():
    model = Sequential([
        Flatten(input_shape=input_shape),               # Flatten input (28x28 to 784)
        Dense(512, activation='relu'),                 # Hidden layer 1 with ReLU
        Dropout(0.3),                                  # Dropout for regularization
        Dense(256, activation='relu'),                 # Hidden layer 2 with ReLU
        Dropout(0.3),                                  # Dropout for regularization
        Dense(128, activation='relu'),                 # Hidden layer 3 with ReLU
        Dense(10, activation='softmax')                # Output layer (10 classes)
    ])
    model.compile(optimizer='adam',                   # Optimizer: Adam
                  loss='categorical_crossentropy',     # Loss function: Categorical Crossentropy
                  metrics=['accuracy'])                # Metric: Accuracy
    return model

# Instantiate the FCNN model
fcnn_model = build_fcnn()

# Train the FCNN
print("Training FCNN...")
fcnn_model.fit(x_train, y_train, epochs=10, batch_size=64, validation_split=0.2)

# Evaluate the FCNN
test_loss, test_accuracy = fcnn_model.evaluate(x_test, y_test, verbose=0)
print(f"Test Accuracy: {test_accuracy * 100:.2f}%")

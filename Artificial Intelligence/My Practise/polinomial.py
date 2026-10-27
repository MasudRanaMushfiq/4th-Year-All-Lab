
# ============================================================
# AI LAB
# Experiment: Deep Neural Network for Polynomial Equation
# Equation: y = 5x^3 - 10x^2 - 20x + 10
#
# Specifications:
# a) Three hidden layers: 32, 64, 128 neurons
# b) Generate samples from -20 to +20 and normalize to [-1, 1]
# c) 80% training, 10% validation, 10% testing
# d) Plot training/validation accuracy and error curves
# ============================================================

import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Input
from tensorflow.keras.callbacks import EarlyStopping
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler

# ------------------------------------------------------------
# 1. Set random seeds for reproducibility
# ------------------------------------------------------------

np.random.seed(42)
tf.random.set_seed(42)

# ------------------------------------------------------------
# 2. Generate 2000 samples in the range [-20, 20]
# ------------------------------------------------------------

x = np.linspace(-20, 20, 2000).reshape(-1, 1)

# Calculate output using the polynomial equation
y = 5 * x**3 - 10 * x**2 - 20 * x + 10

print("Input shape:", x.shape)
print("Output shape:", y.shape)

# ------------------------------------------------------------
# 3. Split raw data into training, validation, and testing sets
#
# 10% test       = 200 samples
# 10% validation = 200 samples
# 80% training   = 1600 samples
# ------------------------------------------------------------

# First, reserve 10% for testing
x_train_val, x_test, y_train_val, y_test = train_test_split(
    x, y,
    test_size=0.10,
    random_state=42
)

# From the remaining 90%, reserve 200 samples for validation.
# 200 / 1800 = 11.111%, giving exactly 10% of the full dataset.
x_train, x_val, y_train, y_val = train_test_split(
    x_train_val, y_train_val,
    test_size=200,
    random_state=42
)

print("\nDataset distribution:")
print("Training samples:", len(x_train))
print("Validation samples:", len(x_val))
print("Testing samples:", len(x_test))

# ------------------------------------------------------------
# 4. Normalize input and output to [-1, +1]
#
# Fit the scalers on training data only to avoid data leakage.
# Apply the fitted scalers to validation and test data.
# ------------------------------------------------------------

x_scaler = MinMaxScaler(feature_range=(-1, 1))
y_scaler = MinMaxScaler(feature_range=(-1, 1))

X_train = x_scaler.fit_transform(x_train)
X_val = x_scaler.transform(x_val)
X_test = x_scaler.transform(x_test)

Y_train = y_scaler.fit_transform(y_train)
Y_val = y_scaler.transform(y_val)
Y_test = y_scaler.transform(y_test)

# ------------------------------------------------------------
# 5. Define a regression tolerance-accuracy metric
#
# A prediction is considered accurate if its normalized
# absolute error is less than 0.05.
# ------------------------------------------------------------

def tolerance_accuracy(y_true, y_pred):
    difference = tf.abs(y_true - y_pred)
    correct = tf.cast(difference < 0.05, tf.float32)
    return tf.reduce_mean(correct)

# ------------------------------------------------------------
# 6. Create the DNN architecture
#
# Input layer:   1 neuron
# Hidden layer 1: 32 neurons
# Hidden layer 2: 64 neurons
# Hidden layer 3: 128 neurons
# Output layer:  1 neuron
# ------------------------------------------------------------

model = Sequential([
    Input(shape=(1,), name="Input_Layer"),
    Dense(32, activation="tanh", name="Hidden_Layer_1"),
    Dense(64, activation="tanh", name="Hidden_Layer_2"),
    Dense(128, activation="tanh", name="Hidden_Layer_3"),
    Dense(1, activation="linear", name="Output_Layer")
])

# Display DNN architecture and number of parameters
print("\nDNN Architecture and Parameters:")
model.summary()

# ------------------------------------------------------------
# 7. Compile the model
#
# Adam: optimizer for updating network weights
# MSE: loss function for measuring regression error
# ------------------------------------------------------------

model.compile(
    optimizer=tf.keras.optimizers.Adam(learning_rate=0.001),
    loss="mse",
    metrics=[tolerance_accuracy]
)

# ------------------------------------------------------------
# 8. Configure early stopping
#
# Maximum epochs = 1000
# Stop if validation error does not improve for 40 epochs.
# Restore weights from the best validation epoch.
# ------------------------------------------------------------

early_stop = EarlyStopping(
    monitor="val_loss",
    patience=40,
    restore_best_weights=True
)

# ------------------------------------------------------------
# 9. Train the DNN
# ------------------------------------------------------------

history = model.fit(
    X_train,
    Y_train,
    epochs=1000,
    batch_size=32,
    validation_data=(X_val, Y_val),
    callbacks=[early_stop],
    verbose=1
)

print("\nTraining completed.")
print("Actual epochs trained:", len(history.history["loss"]))

# ------------------------------------------------------------
# 10. Evaluate the model on the test dataset
# ------------------------------------------------------------

test_loss, test_accuracy = model.evaluate(
    X_test, Y_test, verbose=0
)

print("\nTest Results:")
print("Test MSE:", test_loss)
print("Test tolerance accuracy:", test_accuracy)

# ------------------------------------------------------------
# 11. Plot training accuracy vs validation accuracy
# ------------------------------------------------------------

plt.figure(figsize=(9, 5))

plt.plot(
    history.history["tolerance_accuracy"],
    label="Training Accuracy"
)
plt.plot(
    history.history["val_tolerance_accuracy"],
    label="Validation Accuracy"
)

plt.title("Training Accuracy vs Validation Accuracy")
plt.xlabel("Epoch")
plt.ylabel("Tolerance Accuracy")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# ------------------------------------------------------------
# 12. Plot training error vs validation error
# ------------------------------------------------------------

plt.figure(figsize=(9, 5))

plt.plot(
    history.history["loss"],
    label="Training Error (MSE)"
)
plt.plot(
    history.history["val_loss"],
    label="Validation Error (MSE)"
)

plt.title("Training Error vs Validation Error")
plt.xlabel("Epoch")
plt.ylabel("Mean Squared Error")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# ------------------------------------------------------------
# 13. Plot actual output vs DNN-predicted output
# ------------------------------------------------------------

Y_pred_normalized = model.predict(X_test, verbose=0)

# Convert normalized predictions to original y values
Y_pred = y_scaler.inverse_transform(Y_pred_normalized)

# Sort test points by x for a clear graph
indices = np.argsort(x_test[:, 0])

plt.figure(figsize=(9, 5))

plt.plot(
    x_test[indices, 0],
    y_test[indices, 0],
    label="Actual Values"
)
plt.plot(
    x_test[indices, 0],
    Y_pred[indices, 0],
    "--",
    label="DNN Predictions"
)

plt.title("Actual vs Predicted Polynomial Values")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# ------------------------------------------------------------
# 14. Display five sample predictions
# ------------------------------------------------------------

print("\nSample Predictions:")

for i in range(5):
    print(
        f"x = {x_test[i, 0]:.3f}, "
        f"Actual y = {y_test[i, 0]:.3f}, "
        f"Predicted y = {Y_pred[i, 0]:.3f}"
    )

# ============================================================
# End of AI Lab experiment
# ============================================================


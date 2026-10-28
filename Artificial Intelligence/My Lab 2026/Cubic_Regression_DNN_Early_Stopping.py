import numpy as np
import matplotlib.pyplot as plt

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Input
from sklearn.model_selection import train_test_split
from tensorflow.keras.callbacks import EarlyStopping
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import r2_score
 
def polynomial(x):
    return 5*x**3 - 10*x**2 - 20*x + 10

x = np.linspace(-20, 20, 5000).reshape(-1, 1)
y = polynomial(x)

x_scaler = MinMaxScaler(feature_range=(-1, 1))
y_scaler = MinMaxScaler(feature_range=(-1, 1))
x_normalized = x_scaler.fit_transform(x)
y_normalized = y_scaler.fit_transform(y)

plt.figure(figsize=(8, 5))
plt.plot(x, y)
plt.title("Original Polynomial")
plt.xlabel("x")
plt.ylabel("y")
plt.grid(True)
plt.show()

plt.figure(figsize=(8, 5))
plt.plot(x_normalized, y_normalized)
plt.title("Normalized Polynomial")
plt.xlabel("Normalized x")
plt.ylabel("Normalized y")
plt.grid(True)
plt.show()

x_train, x_temp, y_train, y_temp = train_test_split(x_normalized, y_normalized, test_size=0.20, random_state=42)
x_val, x_test, y_val, y_test = train_test_split(x_temp, y_temp, test_size=0.50, random_state=42)

print("Train:", len(x_train))
print("Validation:", len(x_val))
print("Test:", len(x_test))

model = Sequential([
    Input(shape=(1,)),
    Dense(32, activation="relu"),
    Dense(64, activation="relu"),
    Dense(128, activation="relu"),
    Dense(1)
])
model.summary()

model.compile(
    optimizer="adam",
    loss="mse",
    metrics=["mae"]
)

early_stop = EarlyStopping(
    monitor="val_loss",
    patience=3,
    restore_best_weights=True
)

history = model.fit(
    x_train,
    y_train,
    epochs=100,
    batch_size=32,
    validation_data=(x_val, y_val),
    callbacks=[early_stop],
    verbose=1
)

# print(history.history.keys() 

test_loss, test_mae = model.evaluate(x_test, y_test, verbose=0)
val_loss, val_mae = model.evaluate(x_val, y_val, verbose=0)

print("\n--- Results ---")
print(f"Test Loss (MSE): {test_loss:.4f}")
print(f"Test MAE: {test_mae:.4f}")
print(f"Val Loss (MSE): {val_loss:.4f}")
print(f"Val MAE: {val_mae:.4f}\n")

plt.figure(figsize=(10, 5))
plt.subplot(1, 2, 1)
plt.plot(history.history["loss"], label="Training Loss") #loss means training loss
plt.plot(history.history["val_loss"], label="Validation Loss")
plt.title("Training vs Validation Loss")
plt.xlabel("Epoch")
plt.ylabel("MSE Loss")
plt.legend()
plt.grid(True)
plt.show()

plt.subplot(1, 2, 2)
plt.plot(history.history["mae"], label="Training MAE")
plt.plot(history.history["val_mae"], label="Validation MAE")
plt.title("Training vs Validation MAE")
plt.xlabel("Epoch")
plt.ylabel("MAE")
plt.legend()
plt.grid(True)
plt.show()

print("Real X:", x_scaler.inverse_transform(x_test).flatten()[:5])
print("Pred Y:", y_scaler.inverse_transform(model.predict(x_test, verbose=0)).flatten()[:5])
plt.show()


# Accuracy and error curves
accuracy = 1 - np.array(history.history['mae'])
val_accuracy = 1 - np.array(history.history['val_mae'])
error = history.history['loss']
val_error = history.history['val_loss']

plt.figure(figsize=(12, 4))
plt.subplot(1, 2, 1)
plt.plot(accuracy, label='Training accuracy')
plt.plot(val_accuracy, label='Validation accuracy')
plt.xlabel('Epoch')
plt.ylabel('Accuracy')
plt.title('Training vs Validation Accuracy')

plt.subplot(1, 2, 2)
plt.plot(error, label='Training error')
plt.plot(val_error, label='Validation error')
plt.xlabel('Epoch')
plt.ylabel('MSE')
plt.legend()
plt.title('Training vs Validation Error')
plt.show()


y_pred = model.predict(x_test, verbose=0)
r2 = r2_score(y_test, y_pred)

print("R² Score:", r2)


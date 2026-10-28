import matplotlib.pyplot as plt
import numpy as np
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Input, Dense
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler

def polinomial1(x):
 return 100*x**2 - 20*x + 300

x = np.linspace(-20, 20, 5000).reshape(-1, 1)
y = polinomial1(x)

x_scaler = MinMaxScaler(feature_range=(-1, 1))
y_scaler = MinMaxScaler(feature_range=(-1, 1))

x_normalized = x_scaler.fit_transform(x)
y_normalized = y_scaler.fit_transform(y)

x_train, x_other, y_train, y_other = train_test_split(x_normalized, y_normalized, test_size = 0.20, random_state = 42)
x_test, x_val, y_test, y_val = train_test_split(x_other, y_other, test_size = 0.50, random_state = 42)

model = Sequential([
	Input(shape=(1,)),
	Dense(64, activation="relu"),
	Dense(128, activation="relu"),
	Dense(1)
])
model.summary()

model.compile(
	optimizer='adam',
	loss='mse',
	metrics=['mae']
)

history = model.fit(
	x_train, 
	y_train, 
	epochs = 10,
	batch_size = 32,
	validation_data=(x_val, y_val),
	verbose=1
)
print(history.history.keys())
plt.figure(figsize=(10, 5))
plt.subplot(1,2,1)
plt.plot(history.history['loss'], label="Training Loss")
plt.plot(history.history['val_loss'], label="Validation loss")
plt.title("Training Loss VS Validation Loss")
plt.grid()

plt.subplot(1,2,2)
plt.plot(history.history['mae'], label="Training Loss")
plt.plot(history.history['val_mae'], label="Validation loss")
plt.title("MAE Loss VS Validation Loss")
plt.grid()
plt.show()


test_loss, test_mae = model.evaluate(x_test, y_test, verbose=0)
val_loss, val_mae = model.evaluate(x_val, y_val, verbose=0)

print("\n--- Results ---")
print(f"Test Loss (MSE): {test_loss:.4f}")
print(f"Test MAE: {test_mae:.4f}")
print(f"Val Loss (MSE): {val_loss:.4f}")
print(f"Val MAE: {val_mae:.4f}\n")

accuracy = 1 - np.array(history.history['mae'])
val_accuracy = 1 - np.array(history.history['val_mae'])
print(accuracy)
print(val_accuracy)

print("Hi")


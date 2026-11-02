import java.util.Scanner;

interface Sensor {
    String type();
    double read();
}

class SoilMoistureSensor implements Sensor {
    Scanner sc;

    SoilMoistureSensor(Scanner sc) {
        this.sc = sc;
    }

    public String type() {
        return "Soil Moisture Sensor";
    }

    public double read() {
        System.out.print("Enter soil moisture (%): ");
        return sc.nextDouble();
    }
}

class TemperatureSensor implements Sensor {
    Scanner sc;

    TemperatureSensor(Scanner sc) {
        this.sc = sc;
    }

    public String type() {
        return "Temperature Sensor";
    }

    public double read() {
        System.out.print("Enter temperature: ");
        return sc.nextDouble();
    }
}

class SensorFactory {
    Scanner sc;

    SensorFactory(Scanner sc) {
        this.sc = sc;
    }

    Sensor createSensor(int choice) {
        if (choice == 1)
            return new SoilMoistureSensor(sc);
        else if (choice == 2)
            return new TemperatureSensor(sc);
        else
            return null;
    }
}

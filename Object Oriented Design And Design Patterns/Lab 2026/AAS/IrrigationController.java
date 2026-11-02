class IrrigationController {
    IrrigationStrategy strategy;
    WeatherService weather;
    IrrigationPump pump;
    AlertPublisher alerts;

    IrrigationController(IrrigationStrategy s, WeatherService w,
                         IrrigationPump p, AlertPublisher a) {
        strategy = s;
        weather = w;
        pump = p;
        alerts = a;
    }

    void check(Sensor sensor) {
        double moisture = sensor.read();
        WeatherData data = weather.current();

        System.out.println("\nSensor: " + sensor.type());
        System.out.println("Reading: " + moisture);
        System.out.println("Rain Expected: " + data.rainExpected);

        boolean result = strategy.shouldIrrigate(moisture, data);

        pump.setOn(result);

        if (result)
            alerts.publish("Irrigation Started!");
        else
            alerts.publish("Irrigation Not Required!");
    }
}

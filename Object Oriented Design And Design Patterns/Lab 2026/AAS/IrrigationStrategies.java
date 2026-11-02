interface IrrigationStrategy {
    boolean shouldIrrigate(double moisture, WeatherData weather);
}

class NormalIrrigationStrategy implements IrrigationStrategy {
    public boolean shouldIrrigate(double moisture, WeatherData weather) {
        return moisture < 40 && !weather.rainExpected;
    }
}

class WaterSavingStrategy implements IrrigationStrategy {
    public boolean shouldIrrigate(double moisture, WeatherData weather) {
        return moisture < 25 && !weather.rainExpected;
    }
}

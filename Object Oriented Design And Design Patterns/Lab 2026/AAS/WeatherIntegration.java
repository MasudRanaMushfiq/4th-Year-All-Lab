class WeatherData {
    boolean rainExpected;

    WeatherData(boolean rain) {
        rainExpected = rain;
    }
}

interface WeatherService {
    WeatherData current();
}

class ExternalWeatherAPI {
    boolean getRainForecast(boolean rain) {
        return rain;
    }
}

class WeatherAPIAdapter implements WeatherService {
    ExternalWeatherAPI api;
    boolean rain;

    WeatherAPIAdapter(ExternalWeatherAPI api, boolean rain) {
        this.api = api;
        this.rain = rain;
    }

    public WeatherData current() {
        return new WeatherData(api.getRainForecast(rain));
    }
}

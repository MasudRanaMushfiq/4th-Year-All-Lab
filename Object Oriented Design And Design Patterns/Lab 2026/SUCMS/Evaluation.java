class Marks {
    double obtained, total;

    Marks(double obtained, double total) {
        this.obtained = obtained;
        this.total = total;
    }
}

interface EvaluationStrategy {
    double calculate(Marks m);
}

class TheoryEvaluation implements EvaluationStrategy {
    public double calculate(Marks m) {
        return (m.obtained / m.total) * 100;
    }
}

class LabEvaluation implements EvaluationStrategy {
    public double calculate(Marks m) {
        return (m.obtained / m.total) * 100;
    }
}

class EvaluationService {
    EvaluationStrategy strategy;

    void setStrategy(EvaluationStrategy s) {
        strategy = s;
    }

    double publishResult(Marks m) {
        return strategy.calculate(m);
    }
}

import utils

class Perceptron:
    def __init__(self, legalLabels, max_iterations):
        self.max_iterations = max_iterations
        self.legalLabels = legalLabels
        self.weights = {}
        for label in legalLabels:
            self.weights[label] = utils.Counter()

    def setWeights(self, weights):
        assert len(weights) == len(self.legalLabels)
        self.weights = weights

    def train(self, trainingData, trainingLabels):
        for iteration in range(self.max_iterations):
            print("Starting iteration ", iteration, "...")
            for i in range(len(trainingData)):
                data = trainingData[i]
                actual_label = trainingLabels[i]
                
                # score(f, y) = sigma fi * wi^y
                # sum up the product of weight vector and feature value in each label (dimension of the multiclass perceptron) 
                scores = [(label, self.weights[label]*data) for label in self.legalLabels]
                max_score = max(scores, key = lambda score : scores[1])
                predicted_label = max_score[0]
                
                if predicted_label != actual_label:
                    self.weights[actual_label] += data
                    self.weights[predicted_label] -= data
    
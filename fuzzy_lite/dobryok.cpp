#include "fl/Headers.h"
#include <iostream>


int main(int argc, char* argv[]){
    using namespace fl;
    Engine* engine = new Engine("przeszkody");

	fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("left");
inputVariable1->setRange(0.000, 1023.000);

inputVariable1->addTerm(new fl::Triangle("small", -1023.000, 0.000, 1023.000));
inputVariable1->addTerm(new fl::Triangle("big", 0.000, 1023.000, 2046.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("front");
inputVariable2->setRange(0.000, 1023.000);

inputVariable2->addTerm(new fl::Triangle("small", -1023.000, 0.000, 1023.000));
inputVariable2->addTerm(new fl::Triangle("big", 0.000, 1023.000, 2046.000));
engine->addInputVariable(inputVariable2);

fl::InputVariable* inputVariable3 = new fl::InputVariable;
inputVariable3->setName("right");
inputVariable3->setRange(0.000, 1023.000);

inputVariable3->addTerm(new fl::Triangle("small", -1023.000, 0.000, 1023.000));
inputVariable3->addTerm(new fl::Triangle("big", 0.000, 1023.000, 2046.000));
engine->addInputVariable(inputVariable3);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("v_left");
outputVariable1->setRange(0.000, 1.000);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());


outputVariable1->addTerm(new fl::Constant("front", 10.000));
outputVariable1->addTerm(new fl::Constant("back", -10.000));
engine->addOutputVariable(outputVariable1);

fl::OutputVariable* outputVariable2 = new fl::OutputVariable;
outputVariable2->setName("v_right");
outputVariable2->setRange(0.000, 1.000);
outputVariable2->setDefaultValue(fl::nan);
outputVariable2->setDefuzzifier(new fl::WeightedAverage());


outputVariable2->addTerm(new fl::Constant("front", 10.000));
outputVariable2->addTerm(new fl::Constant("back", -10.000));
engine->addOutputVariable(outputVariable2);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setConjunction(new fl::AlgebraicProduct);
ruleblock1->setDisjunction(new fl::AlgebraicSum);
ruleblock1->setActivation(new fl::Minimum);

ruleblock1->addRule(Rule::parse("if left is small and front is small and right is small then v_left is front and v_right is front", engine));
ruleblock1->addRule(Rule::parse("if left is small and front is small and right is big then v_left is back and v_right is front", engine));
ruleblock1->addRule(Rule::parse("if left is small and front is big and right is small then v_left is back and v_right is front", engine));
ruleblock1->addRule(Rule::parse("if left is small and front is big and right is big then v_left is back and v_right is front", engine));
ruleblock1->addRule(Rule::parse("if left is big and front is small and right is small then v_left is front and v_right is back", engine));
ruleblock1->addRule(Rule::parse("if left is big and front is small and right is big then v_left is front and v_right is front", engine));
ruleblock1->addRule(Rule::parse("if left is big and front is big and right is small then v_left is front and v_right is back", engine));
ruleblock1->addRule(Rule::parse("if left is big and front is big and right is big then v_left is back and v_right is front", engine));
engine->addRuleBlock(ruleblock1);



  //  engine->configure("", "", "Minimum", "Maximum", "Centroid");

	

    std::string status;
    if (! engine->isReady(&status))
		throw Exception("Engine not ready. The following errors were encountered:\n" + status, FL_AT);

    for (int i = 0; i < 50; ++i){
    /*    scalar light1 = inputVariable1->getMinimum() + i * (inputVariable1->range() / 50);
        inputVariable1->setInputValue(light1);

		scalar light2 = inputVariable2->getMinimum() + i * (inputVariable2->range() / 50);
        inputVariable2->setInputValue(light2);

		scalar light3 = inputVariable3->getMinimum() + i * (inputVariable3->range() / 50);
        inputVariable3->setInputValue(light3);
		*/


//	scalar light1;
//	scalar light2;
//	scalar light3;

	    int left=0;
		int right=0;
		int front=0;
		printf("podaj left: ");     
	    scanf("%d", &left ); 
		printf("podaj front: ");
		scanf("%d", &front ); 
		printf("podaj right: ");
		scanf("%d", &right ); 
		 

//	light1=left;
//	light2=right;
//	light3=front;


	inputVariable1->setInputValue(left);
	inputVariable2->setInputValue(front);
	inputVariable3->setInputValue(right);
	

        engine->process();
        FL_LOGP("left.in = " << Op::str(left) << " front.in = " << Op::str(front) << " right.in = "  << Op::str(right) << " -> " <<"Vleft.out = " << Op::str(outputVariable1->getOutputValue()) <<" Vright.out = " << Op::str(outputVariable2->getOutputValue()));   
		

		 //FL_LOG("Ambient.input = " );   
		 

    }
	while(1)
	{
	}
}
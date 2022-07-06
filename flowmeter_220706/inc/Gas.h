#pragma once

class Gas
{
	String m_name;	
	double m_slope;
	double m_intercept;
	double m_secondp;
	double m_maxflow;
	
public:
	
	void setSlope(double s) { m_slope = s; }
	void setIntercept(double i) { m_intercept = i; }
    void setSecondp(double p) { m_secondp = p; }
    void setMaxflow(double m) { m_maxflow = m; }

    double calculateSLM(int voltage) {
		return m_slope * voltage * voltage + m_secondp * voltage + m_intercept;
	}
};
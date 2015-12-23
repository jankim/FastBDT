/**
 * Thomas Keck 2014
 */

#include "FastBDT.h"
#include "IO.h"

#include <gtest/gtest.h>

#include <sstream>
#include <limits>

using namespace FastBDT;

class FeatureBinningTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            std::vector<float> data = {10.0f,8.0f,2.0f,NAN,NAN,NAN,NAN,7.0f,5.0f,6.0f,9.0f,NAN,4.0f,3.0f,11.0f,12.0f,1.0f,NAN};
            calculatedBinning = new FeatureBinning<float>(2, data.begin(), data.end());

            binning = { 1.0f, 7.0f, 4.0f, 10.0f, 12.0f }; 
            predefinedBinning = new FeatureBinning<float>(2, binning.begin(), binning.end());
            
            // Set the binning again, because it is sorted inside the constructor
            binning = { 1.0f, 7.0f, 4.0f, 10.0f, 12.0f }; 
        }

        virtual void TearDown() {
            delete calculatedBinning;
            delete predefinedBinning;
        }

        unsigned int nLevels;
        std::vector<float> binning;
        FeatureBinning<float> *calculatedBinning;
        FeatureBinning<float> *predefinedBinning;

};

TEST_F(FeatureBinningTest, MaximumAndMinimumValueAreCorrectlyIdentified) {

    EXPECT_DOUBLE_EQ( calculatedBinning->GetMin(), 1.0f);
    EXPECT_DOUBLE_EQ( calculatedBinning->GetMax(), 12.0f);
    EXPECT_DOUBLE_EQ( predefinedBinning->GetMin(), 1.0f);
    EXPECT_DOUBLE_EQ( predefinedBinning->GetMax(), 12.0f);

}

TEST_F(FeatureBinningTest, NumberOfLevelsAndBinsIsCorrectlyIdentified) {

    EXPECT_EQ( calculatedBinning->GetNLevels(), 2u );
    EXPECT_EQ( predefinedBinning->GetNLevels(), 2u );
    // 5 bins, 2^2 ordinary bins + 1 NaN bin
    EXPECT_EQ( calculatedBinning->GetNBins(), 5u );
    EXPECT_EQ( predefinedBinning->GetNBins(), 5u );

}

TEST_F(FeatureBinningTest, ValueToBinMapsNormalValuesCorrectly) {

    EXPECT_EQ( calculatedBinning->ValueToBin(1.0f), 1u);
    EXPECT_EQ( calculatedBinning->ValueToBin(2.0f), 1u);
    EXPECT_EQ( calculatedBinning->ValueToBin(3.0f), 1u);
    EXPECT_EQ( calculatedBinning->ValueToBin(4.0f), 2u);
    EXPECT_EQ( calculatedBinning->ValueToBin(5.0f), 2u);
    EXPECT_EQ( calculatedBinning->ValueToBin(6.0f), 2u);
    EXPECT_EQ( calculatedBinning->ValueToBin(7.0f), 3u);
    EXPECT_EQ( calculatedBinning->ValueToBin(8.0f), 3u);
    EXPECT_EQ( calculatedBinning->ValueToBin(9.0f), 3u);
    EXPECT_EQ( calculatedBinning->ValueToBin(10.0f), 4u);
    EXPECT_EQ( calculatedBinning->ValueToBin(11.0f), 4u);
    EXPECT_EQ( calculatedBinning->ValueToBin(12.0f), 4u);
    
    EXPECT_EQ( predefinedBinning->ValueToBin(1.0f), 1u);
    EXPECT_EQ( predefinedBinning->ValueToBin(2.0f), 1u);
    EXPECT_EQ( predefinedBinning->ValueToBin(3.0f), 1u);
    EXPECT_EQ( predefinedBinning->ValueToBin(4.0f), 2u);
    EXPECT_EQ( predefinedBinning->ValueToBin(5.0f), 2u);
    EXPECT_EQ( predefinedBinning->ValueToBin(6.0f), 2u);
    EXPECT_EQ( predefinedBinning->ValueToBin(7.0f), 3u);
    EXPECT_EQ( predefinedBinning->ValueToBin(8.0f), 3u);
    EXPECT_EQ( predefinedBinning->ValueToBin(9.0f), 3u);
    EXPECT_EQ( predefinedBinning->ValueToBin(10.0f), 4u);
    EXPECT_EQ( predefinedBinning->ValueToBin(11.0f), 4u);
    EXPECT_EQ( predefinedBinning->ValueToBin(12.0f), 4u);

}

TEST_F(FeatureBinningTest, NaNGivesZeroBin) {

    EXPECT_EQ( predefinedBinning->ValueToBin(NAN), 0u);
    EXPECT_EQ( predefinedBinning->ValueToBin(NAN), 0u);

}

TEST_F(FeatureBinningTest, OverflowAndUnderflowGivesLastAndFirstBin) {

    EXPECT_EQ( calculatedBinning->ValueToBin(100.0f), 4u);
    EXPECT_EQ( calculatedBinning->ValueToBin(-100.0f), 1u);
    EXPECT_EQ( predefinedBinning->ValueToBin(100.0f), 4u);
    EXPECT_EQ( predefinedBinning->ValueToBin(-100.0f), 1u);

}

TEST_F(FeatureBinningTest, UsingMaximumOfDoubleIsSafe) {

    EXPECT_EQ( calculatedBinning->ValueToBin(std::numeric_limits<float>::max()), 4u);
    EXPECT_EQ( calculatedBinning->ValueToBin(std::numeric_limits<float>::lowest()), 1u);
    EXPECT_EQ( predefinedBinning->ValueToBin(std::numeric_limits<float>::max()), 4u);
    EXPECT_EQ( predefinedBinning->ValueToBin(std::numeric_limits<float>::lowest()), 1u);

}


TEST_F(FeatureBinningTest, UsingInfinityIsSafe) {

    EXPECT_EQ( calculatedBinning->ValueToBin(std::numeric_limits<float>::infinity()), 4u);
    EXPECT_EQ( calculatedBinning->ValueToBin(-std::numeric_limits<float>::infinity()), 1u);
    EXPECT_EQ( predefinedBinning->ValueToBin(std::numeric_limits<float>::infinity()), 4u);
    EXPECT_EQ( predefinedBinning->ValueToBin(-std::numeric_limits<float>::infinity()), 1u);

}

TEST_F(FeatureBinningTest, GetBinningIsCorrect) {

    EXPECT_EQ( calculatedBinning->GetBinning(), binning);
    EXPECT_EQ( predefinedBinning->GetBinning(), binning);

}

TEST_F(FeatureBinningTest, ConstantFeatureIsHandledCorrectly) {

    std::vector<float> data = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }; 
    FeatureBinning<float> featureBinning(3, data.begin(), data.end());

    std::vector<float> binning = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }; 
    EXPECT_EQ( featureBinning.GetNBins(), 9u);
    EXPECT_EQ( featureBinning.GetBinning(), binning);
    EXPECT_EQ( featureBinning.ValueToBin(100.0f), 8u);
    EXPECT_EQ( featureBinning.ValueToBin(-100.0f), 1u);
    EXPECT_EQ( featureBinning.ValueToBin(1.0f), 8u);

}

TEST_F(FeatureBinningTest, LowStatisticIsHandledCorrectly) {

    std::vector<float> data = { 1.0f, 4.0f, 4.0f, 7.0f, 10.0f, 11.0f, 12.0f }; 
    FeatureBinning<float> featureBinning(3, data.begin(), data.end());

    std::vector<float> binning = { 1.0f, 7.0f, 4.0f, 10.0f, 1.0f, 4.0f, 7.0f, 11.0f, 12.0f }; 
    EXPECT_EQ( featureBinning.GetNBins(), 9u);
    EXPECT_EQ( featureBinning.GetBinning(), binning);
    
    EXPECT_EQ( featureBinning.ValueToBin(100.0f), 8u);
    EXPECT_EQ( featureBinning.ValueToBin(-100.0f), 1u);
    
    EXPECT_EQ( featureBinning.ValueToBin(1.0f), 2u);
    EXPECT_EQ( featureBinning.ValueToBin(2.0f), 2u);
    EXPECT_EQ( featureBinning.ValueToBin(3.0f), 2u);
    EXPECT_EQ( featureBinning.ValueToBin(4.0f), 4u);
    EXPECT_EQ( featureBinning.ValueToBin(5.0f), 4u);
    EXPECT_EQ( featureBinning.ValueToBin(6.0f), 4u);
    EXPECT_EQ( featureBinning.ValueToBin(7.0f), 6u);
    EXPECT_EQ( featureBinning.ValueToBin(8.0f), 6u);
    EXPECT_EQ( featureBinning.ValueToBin(9.0f), 6u);
    EXPECT_EQ( featureBinning.ValueToBin(10.0f), 7u);
    EXPECT_EQ( featureBinning.ValueToBin(11.0f), 8u);
    EXPECT_EQ( featureBinning.ValueToBin(12.0f), 8u);
    
    FeatureBinning<float> featureBinning2(4, data.begin(), data.end());

    std::vector<float> binning2 = { 1.0f, 7.0f, 4.0f, 10.0f, 1.0f, 4.0f, 7.0f, 11.0f, 1.0f, 1.0f, 4.0f, 4.0f, 7.0f, 10.0f, 11.0f, 12.0f, 12.0f }; 
    EXPECT_EQ( featureBinning2.GetNBins(), 17u);
    EXPECT_EQ( featureBinning2.GetBinning(), binning2);
    
    EXPECT_EQ( featureBinning2.ValueToBin(100.0f), 16u);
    EXPECT_EQ( featureBinning2.ValueToBin(-100.0f), 1u);
    
    EXPECT_EQ( featureBinning2.ValueToBin(1.0f), 4u);
    EXPECT_EQ( featureBinning2.ValueToBin(2.0f), 4u);
    EXPECT_EQ( featureBinning2.ValueToBin(3.0f), 4u);
    EXPECT_EQ( featureBinning2.ValueToBin(4.0f), 8u);
    EXPECT_EQ( featureBinning2.ValueToBin(5.0f), 8u);
    EXPECT_EQ( featureBinning2.ValueToBin(6.0f), 8u);
    EXPECT_EQ( featureBinning2.ValueToBin(7.0f), 11u);
    EXPECT_EQ( featureBinning2.ValueToBin(8.0f), 11u);
    EXPECT_EQ( featureBinning2.ValueToBin(9.0f), 11u);
    EXPECT_EQ( featureBinning2.ValueToBin(10.0f), 13u);
    EXPECT_EQ( featureBinning2.ValueToBin(11.0f), 15u);
    EXPECT_EQ( featureBinning2.ValueToBin(12.0f), 16u);
    
}


class EventWeightsTest : public ::testing::Test {

    protected:
        virtual void SetUp() {
            eventWeights = new EventWeights(10);
            for(unsigned int i = 0; i < 10; ++i) {
                eventWeights->Set(i, static_cast<float>(i+1));
                eventWeights->SetOriginal(i, 2);
            }
        }

        virtual void TearDown() {
            delete eventWeights;
        }

        EventWeights *eventWeights;
};

TEST_F(EventWeightsTest, WeightSumsAreCorrect) {

    auto sums = eventWeights->GetSums(5);
    EXPECT_DOUBLE_EQ(sums[0], 15.0 * 2);
    EXPECT_DOUBLE_EQ(sums[1], 40.0 * 2);
    EXPECT_DOUBLE_EQ(sums[2], 385.0 * 2);

}

TEST_F(EventWeightsTest, WeightSumsAreNotInfluencedByZeroWeights) {

    auto sums = eventWeights->GetSums(5);
            
    EventWeights *newEventWeights = new EventWeights(20);
    for(unsigned int i = 0; i < 10; ++i) {
        // Get delivers the weight*original weight, therefore we need to divide by the original weight afterwards
        newEventWeights->Set(i*2, eventWeights->Get(i) / eventWeights->GetOriginal(i));
        newEventWeights->SetOriginal(i*2, eventWeights->GetOriginal(i));
        newEventWeights->Set(i*2 + 1, 0.0);
        newEventWeights->SetOriginal(i*2 + 1, 0.0);
    }
    auto newSums = newEventWeights->GetSums(10);
    delete newEventWeights;

    EXPECT_DOUBLE_EQ(sums[0], newSums[0]);
    EXPECT_DOUBLE_EQ(sums[1], newSums[1]);
    EXPECT_DOUBLE_EQ(sums[2], newSums[2]);

}

TEST_F(EventWeightsTest, GetterIsCorrect) {

    for(unsigned int i = 0; i < 10; ++i) {
        EXPECT_DOUBLE_EQ( eventWeights->Get(i), static_cast<float>(i+1) * 2); 
    }
    
}

TEST_F(EventWeightsTest, WeightSumsAndGetterAreCorrectlyUpdated) {

    for(unsigned int i = 0; i < 10; ++i) {
        eventWeights->Set(i, static_cast<float>(i+3));
    }

    auto sums = eventWeights->GetSums(5);
    EXPECT_DOUBLE_EQ(sums[0], 25.0 * 2);
    EXPECT_DOUBLE_EQ(sums[1], 50.0 * 2);
    EXPECT_DOUBLE_EQ(sums[2], 645.0 * 2);
    
    for(unsigned int i = 0; i < 10; ++i) {
        EXPECT_DOUBLE_EQ( eventWeights->Get(i), static_cast<float>(i+3) * 2); 
    }

}

class EventFlagsTest : public ::testing::Test {

    protected:
        virtual void SetUp() {
            eventFlags = new EventFlags(10);
        }

        virtual void TearDown() {
            delete eventFlags;
        }

        EventFlags *eventFlags;
};

TEST_F(EventFlagsTest, IsInitialisedWithOnes) {

    for(unsigned int i = 0; i < 10; ++i)
        EXPECT_EQ( eventFlags->Get(i), 1);

}

TEST_F(EventFlagsTest, SetterAndGetterWorkCorrectly) {
    
    for(unsigned int i = 0; i < 10; ++i)
        eventFlags->Set(i, i-5 );

    for(unsigned int i = 0; i < 10; ++i)
        EXPECT_EQ( eventFlags->Get(i), static_cast<int>(i)-5);

}

class EventValuesTest : public ::testing::Test {

    protected:
        virtual void SetUp() {
            eventValues = new EventValues(8, 4, {3, 4, 2, 3});
        }

        virtual void TearDown() {
            delete eventValues;
        }

        EventValues *eventValues;
};

TEST_F(EventValuesTest, SetterAndGetterWorkCorrectly) {
    
    for(unsigned int i = 0; i < 8; ++i) {
        std::vector<unsigned int> features = { i, static_cast<unsigned int>(4 + (1-2*((int)i%2))*((int)i+1)/2), static_cast<unsigned int>((int)(i) % 4 + 1),  7-i };
        eventValues->Set(i, features);
    }
    EXPECT_THROW( eventValues->Set(1, {1,2,3,4,5}), std::runtime_error );
    EXPECT_THROW( eventValues->Set(1, {1,20,3,1}), std::runtime_error );
    
    for(unsigned int i = 0; i < 8; ++i) {
        std::vector<unsigned int> features = { i, static_cast<unsigned int>(4 + (1-2*((int)(i)%2))*((int)(i)+1)/2), static_cast<unsigned int>((int)(i) % 4 + 1),  7-i };
        const auto *array = &eventValues->Get(i);
        for(unsigned int j = 0; j < 3; ++j) {
            EXPECT_EQ( eventValues->Get(i,j), features[j]);
            EXPECT_EQ( array[j], features[j]);
        }
    }
}


TEST_F(EventValuesTest, ThrowOnMismatchBetweenNFeaturesAndNBinsSize) {
    
  EXPECT_THROW( EventValues(8, 3, {1, 2}), std::runtime_error );

}


TEST_F(EventValuesTest, GetSizesWorkCorrectly) {

    EXPECT_EQ( eventValues->GetNFeatures(), 4u);
    const auto& nBins = eventValues->GetNBins();
    EXPECT_EQ( nBins.size(), 4u);
    EXPECT_EQ( nBins[0], 9u);
    EXPECT_EQ( nBins[1], 17u);
    EXPECT_EQ( nBins[2], 5u);
    EXPECT_EQ( nBins[3], 9u);
    
    const auto& nBinSums = eventValues->GetNBinSums();
    EXPECT_EQ( nBinSums.size(), 5u);
    EXPECT_EQ( nBinSums[0], 0u);
    EXPECT_EQ( nBinSums[1], 9u);
    EXPECT_EQ( nBinSums[2], 9u + 17u);
    EXPECT_EQ( nBinSums[3], 9u + 17u + 5u);
    EXPECT_EQ( nBinSums[4], 9u + 17u + 5u + 9u);

}

class EventSampleTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            eventSample = new EventSample(10, 3, {8, 8, 8});
        }

        virtual void TearDown() {
            delete eventSample;
        }

        EventSample *eventSample;

};

TEST_F(EventSampleTest, AddingEventsWorksCorrectly) {

    eventSample->AddEvent( std::vector<unsigned int>({1,2,3}), 2.0, true );
    EXPECT_EQ( eventSample->GetNSignals(), 1u);
    EXPECT_EQ( eventSample->GetNBckgrds(), 0u);
   
    const auto &eventWeights = eventSample->GetWeights();
    auto sums = eventWeights.GetSums(5);
    EXPECT_DOUBLE_EQ(sums[0], 2.0);
    EXPECT_DOUBLE_EQ(sums[1], 0.0);

 
    // Add some more Signal and Background events   
    for(unsigned int i = 1; i < 10; ++i) { 
        eventSample->AddEvent( std::vector<unsigned int>({2*i,3*i,5*i}), 2.0, i % 2 == 0 );
    }
    EXPECT_EQ( eventSample->GetNSignals(), 5u);
    EXPECT_EQ( eventSample->GetNBckgrds(), 5u);
    
    sums = eventWeights.GetSums(5);
    EXPECT_DOUBLE_EQ(sums[0], 10.0);
    EXPECT_DOUBLE_EQ(sums[1], 10.0);
    
    // Test some of the values, if they're correct
    // Remember that the events are NOT in the same order as they were added,
    // instead the signal events are added starting from 0, and the background events
    // are added reversed starting from the last event.
    EXPECT_EQ( eventSample->GetValues().Get(1,2), 10u); 
    EXPECT_EQ( eventSample->GetValues().Get(3,1), 18u); 
    EXPECT_EQ( eventSample->GetValues().Get(9,0), 2u); 

    // Test if signal and background labels are correctly assigned
    for(unsigned int i = 0; i < 5; ++i) {
        EXPECT_TRUE( eventSample->IsSignal(i));
        EXPECT_FALSE( eventSample->IsSignal(i+5));
    }

    // Test throw if number of promised events is exceeded
    EXPECT_THROW( eventSample->AddEvent( std::vector<unsigned int>({1,2,3}), 2.0, true ), std::runtime_error);
    
}

TEST_F(EventSampleTest, AddingEventsWithZeroWeightWorksCorrectly) {
 
    // Add some more Signal and Background events   
    for(unsigned int i = 0; i < 10; ++i) { 
        eventSample->AddEvent( std::vector<unsigned int>({2*i,3*i,5*i}), i % 3, i % 2 == 0 );
    }
    EXPECT_EQ( eventSample->GetNSignals(), 5u);
    EXPECT_EQ( eventSample->GetNBckgrds(), 5u);
    
    const auto &eventWeights = eventSample->GetWeights();
    const auto& sums = eventWeights.GetSums(5);
    EXPECT_DOUBLE_EQ(sums[0], 5.0);
    EXPECT_DOUBLE_EQ(sums[1], 4.0);

}

class CumulativeDistributionsTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            const unsigned int numberOfEvents = 100;
            eventSample = new EventSample(numberOfEvents, 2, {2, 2});
            for(unsigned int i = 0; i < numberOfEvents; ++i) {
                bool isSignal = i < (numberOfEvents/2);
                eventSample->AddEvent( std::vector<unsigned int>({i % 4 + 1, (numberOfEvents-i) % 4 + 1}), static_cast<float>(i+1), isSignal);
            }
        }

        virtual void TearDown() {
            delete eventSample;
        }

        EventSample *eventSample;
};

TEST_F(CumulativeDistributionsTest, CheckIfLayer0IsCorrect) {

    CumulativeDistributions CDFsForLayer0(0, *eventSample);

    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, 1), 325.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, 2), 663.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, 3), 963.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, 4), 1275.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 1, 1), 325.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 1, 2), 637.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 1, 3), 937.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 1, 4), 1275.0); 

    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, 1), 900.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, 2), 1812.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, 3), 2787.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, 4), 3775.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 1, 1), 900.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 1, 2), 1888.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 1, 3), 2863.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 1, 4), 3775.0); 

}

TEST_F(CumulativeDistributionsTest, NaNShouldBeIgnored) {

    CumulativeDistributions CDFsForLayer0(0, *eventSample);
            
    std::vector<unsigned int> v(2);
    EventSample *newEventSample = new EventSample(200, 2, {2, 2});
    for(unsigned int i = 0; i < 100; ++i) {
        v[0] = eventSample->GetValues().Get(i, 0);
        v[1] = eventSample->GetValues().Get(i, 1);
        newEventSample->AddEvent(v, eventSample->GetWeights().GetOriginal(i), eventSample->IsSignal(i));
        newEventSample->AddEvent(std::vector<unsigned int>({0, 0}), 1.0, i < 50);
    }
    CumulativeDistributions newCDFsForLayer0(0, *newEventSample);
    delete newEventSample;

    for(unsigned int iBin = 1; iBin < 5; ++iBin) {
      EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, iBin), newCDFsForLayer0.GetSignal(0, 0, iBin)); 
      EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, iBin), newCDFsForLayer0.GetBckgrd(0, 0, iBin)); 
      EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 1, iBin), newCDFsForLayer0.GetSignal(0, 1, iBin)); 
      EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 1, iBin), newCDFsForLayer0.GetBckgrd(0, 1, iBin)); 
    }

    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, 0), 0.0);
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, 0), 0.0);
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 1, 0), 0.0);
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 1, 0), 0.0);
    
    EXPECT_DOUBLE_EQ( newCDFsForLayer0.GetSignal(0, 0, 0), 50.0);
    EXPECT_DOUBLE_EQ( newCDFsForLayer0.GetBckgrd(0, 0, 0), 50.0);
    EXPECT_DOUBLE_EQ( newCDFsForLayer0.GetSignal(0, 1, 0), 50.0);
    EXPECT_DOUBLE_EQ( newCDFsForLayer0.GetBckgrd(0, 1, 0), 50.0);

}

TEST_F(CumulativeDistributionsTest, ZeroWeightShouldBeIgnored) {

    CumulativeDistributions CDFsForLayer0(0, *eventSample);
            
    std::vector<unsigned int> v(2);
    EventSample *newEventSample = new EventSample(200, 2, {2, 2});
    for(unsigned int i = 0; i < 100; ++i) {
        v[0] = eventSample->GetValues().Get(i, 0);
        v[1] = eventSample->GetValues().Get(i, 1);
        newEventSample->AddEvent(v, eventSample->GetWeights().GetOriginal(i), eventSample->IsSignal(i));
        newEventSample->AddEvent(std::vector<unsigned int>({i%2 + 1, i%3 + 1}), 0.0, i < 50);
    }
    CumulativeDistributions newCDFsForLayer0(0, *newEventSample);
    delete newEventSample;

    for(unsigned int iBin = 0; iBin < 5; ++iBin) {
      EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, iBin), newCDFsForLayer0.GetSignal(0, 0, iBin)); 
      EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, iBin), newCDFsForLayer0.GetBckgrd(0, 0, iBin)); 
      EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 1, iBin), newCDFsForLayer0.GetSignal(0, 1, iBin)); 
      EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 1, iBin), newCDFsForLayer0.GetBckgrd(0, 1, iBin)); 
    }

}


TEST_F(CumulativeDistributionsTest, CheckIfLayer1IsCorrect) {
    
    auto &eventFlags = eventSample->GetFlags();
    for(unsigned int i = 0; i < 50; ++i) {
        eventFlags.Set(i, i%2 + 2 );
    }
    for(unsigned int i = 50; i < 100; ++i) {
        eventFlags.Set(149-i, i%2 + 2 );
    }

    CumulativeDistributions CDFsForLayer1(1, *eventSample);

    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 0, 1), 325.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 0, 2), 325.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 0, 3), 625.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 0, 4), 625.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 1, 1), 325.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 1, 2), 325.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 1, 3), 625.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 1, 4), 625.0); 

    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 0, 1), 900.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 0, 2), 900.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 0, 3), 1875.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 0, 4), 1875.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 1, 1), 900.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 1, 2), 900.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 1, 3), 1875.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 1, 4), 1875.0); 

    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 0, 1), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 0, 2), 338.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 0, 3), 338.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 0, 4), 650.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 1, 1), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 1, 2), 312.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 1, 3), 312.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 1, 4), 650.0); 

    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 0, 1), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 0, 2), 912.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 0, 3), 912.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 0, 4), 1900.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 1, 1), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 1, 2), 988.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 1, 3), 988.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 1, 4), 1900.0); 

}

TEST_F(CumulativeDistributionsTest, DifferentBinningLevels) {
    const unsigned int numberOfEvents = 10;
    EventSample *sample = new EventSample(numberOfEvents, 4, {2, 1, 3, 1});
    sample->AddEvent(std::vector<unsigned int>{3, 1, 8, 2}, 1.0, true); 
    sample->AddEvent(std::vector<unsigned int>{4, 2, 7, 2}, 1.0, true); 
    sample->AddEvent(std::vector<unsigned int>{3, 2, 6, 0}, 1.0, true); 
    sample->AddEvent(std::vector<unsigned int>{2, 1, 5, 1}, 1.0, true); 
    sample->AddEvent(std::vector<unsigned int>{1, 1, 4, 1}, 1.0, true); 
    sample->AddEvent(std::vector<unsigned int>{3, 1, 3, 2}, 1.0, false); 
    sample->AddEvent(std::vector<unsigned int>{4, 2, 2, 2}, 1.0, false); 
    sample->AddEvent(std::vector<unsigned int>{3, 2, 1, 0}, 1.0, false); 
    sample->AddEvent(std::vector<unsigned int>{2, 1, 2, 1}, 1.0, false); 
    sample->AddEvent(std::vector<unsigned int>{1, 1, 3, 2}, 1.0, false); 
    
    CumulativeDistributions CDFsForLayer0(0, *sample);

    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, 0), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, 1), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, 2), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, 3), 4.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 0, 4), 5.0);

    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 1, 0), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 1, 1), 3.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 1, 2), 5.0); 
    
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 2, 0), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 2, 1), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 2, 2), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 2, 3), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 2, 4), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 2, 5), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 2, 6), 3.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 2, 7), 4.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 2, 8), 5.0);  

    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, 0), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 3, 0), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 3, 1), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetSignal(0, 3, 2), 4.0); 
    
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, 1), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, 2), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, 3), 4.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 0, 4), 5.0); 
    
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 1, 0), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 1, 1), 3.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 1, 2), 5.0); 
    
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 2, 0), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 2, 1), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 2, 2), 3.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 2, 3), 5.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 2, 4), 5.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 2, 5), 5.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 2, 6), 5.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 2, 7), 5.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 2, 8), 5.0); 
    
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 3, 0), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 3, 1), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer0.GetBckgrd(0, 3, 2), 4.0); 
    
    auto &eventFlags = sample->GetFlags();
    for(unsigned int i = 0; i < 10; ++i) {
      eventFlags.Set(i, i%2 + 2);
    }

    // We check only the third feature here, if something goes wrong
    // due to the different binning sizes this should influence this feature.
    CumulativeDistributions CDFsForLayer1(1, *sample);
    
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 2, 0), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 2, 1), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 2, 2), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 2, 3), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 2, 4), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 2, 5), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 2, 6), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 2, 7), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(0, 2, 8), 3.0);  
    
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 2, 0), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 2, 1), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 2, 2), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 2, 3), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 2, 4), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 2, 5), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 2, 6), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 2, 7), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(0, 2, 8), 2.0); 
    
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 2, 0), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 2, 1), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 2, 2), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 2, 3), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 2, 4), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 2, 5), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 2, 6), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 2, 7), 2.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetSignal(1, 2, 8), 2.0);  
    
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 2, 0), 0.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 2, 1), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 2, 2), 1.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 2, 3), 3.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 2, 4), 3.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 2, 5), 3.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 2, 6), 3.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 2, 7), 3.0); 
    EXPECT_DOUBLE_EQ( CDFsForLayer1.GetBckgrd(1, 2, 8), 3.0); 

    delete sample;
}

class LossFunctionTest : public ::testing::Test { };

TEST_F(LossFunctionTest, GiniIndexIsCorrect) {

    EXPECT_DOUBLE_EQ( LossFunction(4,4), 2.0);
    EXPECT_DOUBLE_EQ( LossFunction(1,4), 0.8);
    EXPECT_DOUBLE_EQ( LossFunction(4,1), 0.8);
    EXPECT_DOUBLE_EQ( LossFunction(2,0), 0.0);
    EXPECT_DOUBLE_EQ( LossFunction(0,2), 0.0);

}

class NodeTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            eventSample = new EventSample(8, 2, {1, 1});
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 1 }), 4.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 2 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 1 }), 4.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 2 }), 3.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 1 }), 2.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 2 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 1 }), 3.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 2 }), 2.0, false);
            
        }

        virtual void TearDown() {
            delete eventSample;
        }

        EventSample *eventSample;
};

TEST_F(NodeTest, IsInLayerIsCorrect) {

    EXPECT_TRUE( Node(0,0).IsInLayer(0) );
    EXPECT_FALSE( Node(0,0).IsInLayer(1) );
    EXPECT_TRUE( Node(1,0).IsInLayer(1) );
    EXPECT_FALSE( Node(1,0).IsInLayer(0) );
    EXPECT_FALSE( Node(1,0).IsInLayer(2) );

}

TEST_F(NodeTest, PositionIsCorrectlyDetermined) {

    EXPECT_EQ( Node(0,0).GetPosition(), 0u );
    EXPECT_EQ( Node(1,0).GetPosition(), 1u );
    EXPECT_EQ( Node(1,1).GetPosition(), 2u );
    EXPECT_EQ( Node(2,0).GetPosition(), 3u );
    EXPECT_EQ( Node(2,1).GetPosition(), 4u );
    EXPECT_EQ( Node(2,2).GetPosition(), 5u );
    EXPECT_EQ( Node(2,3).GetPosition(), 6u );
    EXPECT_EQ( Node(3,0).GetPosition(), 7u );
    EXPECT_EQ( Node(3,7).GetPosition(), 14u );

}

TEST_F(NodeTest, BoostWeightCalculation) {

    Node node(0,0);
    node.SetWeights({2.0, 2.0, 4.0});
    EXPECT_DOUBLE_EQ(node.GetBoostWeight(), 0.0); 
    node.SetWeights({0.0, 0.0, 0.0});
    node.AddSignalWeight(1.0, 1.0);
    node.AddSignalWeight(3.0, 1.0);
    node.AddBckgrdWeight(2.0, 1.0);
    EXPECT_DOUBLE_EQ(node.GetBoostWeight(), -1.0);

}

TEST_F(NodeTest, PurityCalculation) {

    Node node(0,0);
    node.SetWeights({2.0, 2.0, 4.0});
    EXPECT_DOUBLE_EQ(node.GetPurity(), 0.5); 
    node.SetWeights({0.0, 0.0, 0.0});
    node.AddSignalWeight(2.0, 1.0);
    node.AddSignalWeight(4.0, 1.0);
    node.AddBckgrdWeight(4.0, 1.0);
    EXPECT_DOUBLE_EQ(node.GetPurity(), 0.6);

}

TEST_F(NodeTest, NegativeWeightsAreHandledCorrectly) {

    Node node(0,0);
    node.SetWeights({0.0, 0.0, 0.0});
    node.AddSignalWeight(-2.0, -1.0);
    node.AddSignalWeight(-4.0, -1.0);
    node.AddBckgrdWeight(-4.0, -1.0);
    EXPECT_DOUBLE_EQ(node.GetPurity(), 0.6);
    EXPECT_DOUBLE_EQ(node.GetBoostWeight(), -0.125);
    
    node.SetWeights({0.0, 0.0, 0.0});
    node.AddSignalWeight(-2.0, 1.0);
    node.AddSignalWeight(1.0, -2.0);
    node.AddBckgrdWeight(0.5, -0.5);
    // Purity above 1.0 can happen with negative weights
    EXPECT_DOUBLE_EQ(node.GetPurity(), 2.0);
    EXPECT_DOUBLE_EQ(node.GetBoostWeight(), 0.375);

}


TEST_F(NodeTest, AddZeroWeightDoesNotChangeAnything) {

    Node node(0,0);
    node.SetWeights({0.0, 0.0, 0.0});
    node.AddSignalWeight(2.0, 1.0);
    node.AddSignalWeight(2.0, -1.0);
    node.AddSignalWeight(4.0, 1.0);
    node.AddSignalWeight(-4.0, 2.0);
    node.AddBckgrdWeight(4.0, 1.0);
    node.AddBckgrdWeight(4.0, 1.0);
    node.AddBckgrdWeight(3.0, -1.0);
    node.AddBckgrdWeight(2.0, 2.0);
    node.AddBckgrdWeight(0.5, 0.1);
    
    Node newNode(0,0);
    newNode.SetWeights({0.0, 0.0, 0.0});
    newNode.AddSignalWeight(2.0, 1.0);
    newNode.AddSignalWeight(2.0, -1.0);
    newNode.AddSignalWeight(2.0, 0.0);
    newNode.AddSignalWeight(4.0, 1.0);
    newNode.AddSignalWeight(-4.0, 2.0);
    newNode.AddSignalWeight(-4.0, 0.0);
    newNode.AddBckgrdWeight(4.0, 1.0);
    newNode.AddBckgrdWeight(4.0, 0.0);
    newNode.AddBckgrdWeight(4.0, 1.0);
    newNode.AddBckgrdWeight(3.0, -1.0);
    newNode.AddBckgrdWeight(2.0, 2.0);
    newNode.AddBckgrdWeight(0.0, 0.0);
    newNode.AddBckgrdWeight(0.5, 0.1);
    

    EXPECT_DOUBLE_EQ(node.GetPurity(), newNode.GetPurity());
    EXPECT_DOUBLE_EQ(node.GetBoostWeight(), newNode.GetBoostWeight());

}

TEST_F(NodeTest, BestCut0Layer) {

    CumulativeDistributions CDFs(0, *eventSample);
    Node node(0,0);
    node.SetWeights({10.0, 10.0, 68.0});

    Cut bestCut = node.CalculateBestCut(CDFs);
    EXPECT_EQ( bestCut.feature, 0u );
    EXPECT_EQ( bestCut.index, 2u );
    EXPECT_DOUBLE_EQ( bestCut.gain, 1.875 );
    EXPECT_TRUE( bestCut.valid );

}

TEST_F(NodeTest, NaNIsIgnored) {

    CumulativeDistributions CDFs(0, *eventSample);
    Node node(0,0);
    node.SetWeights({10.0, 10.0, 68.0});
    Cut bestCut = node.CalculateBestCut(CDFs);
    
    EXPECT_DOUBLE_EQ(CDFs.GetSignal(0, 0, 0), 0.0);
    EXPECT_DOUBLE_EQ(CDFs.GetBckgrd(0, 0, 0), 0.0);
    EXPECT_DOUBLE_EQ(CDFs.GetSignal(0, 1, 0), 0.0);
    EXPECT_DOUBLE_EQ(CDFs.GetBckgrd(0, 1, 0), 0.0);
    // I violate constness here because it's the simplest way to test the influence
    // of the 0th bin, which contains the weights for the NaN values.
    // Signal and Background are chosen extremly asymmetric for both features, so
    // this should change the cut if the 0th bin is considered.
    const_cast<float&>(CDFs.GetSignal(0, 0, 0)) = 100.0;
    const_cast<float&>(CDFs.GetBckgrd(0, 0, 0)) = 1.0;
    const_cast<float&>(CDFs.GetSignal(0, 1, 0)) = 10.0;
    const_cast<float&>(CDFs.GetBckgrd(0, 1, 0)) = 800.0;
    Cut newBestCut = node.CalculateBestCut(CDFs);

    EXPECT_EQ( bestCut.feature, newBestCut.feature );
    EXPECT_EQ( bestCut.index, newBestCut.index );
    EXPECT_DOUBLE_EQ( bestCut.gain, newBestCut.gain );
    EXPECT_EQ( bestCut.valid, newBestCut.valid );

}

TEST_F(NodeTest, BestCut1Layer) {

    auto &flags = eventSample->GetFlags();
    flags.Set(0, 2);
    flags.Set(1, 2);
    flags.Set(2, 2);
    flags.Set(3, 3);
    flags.Set(4, 3);
    flags.Set(5, 2);
    flags.Set(6, 3);
    flags.Set(7, 3);

    CumulativeDistributions CDFs(1, *eventSample);

    Node right_node(1,0);
    right_node.SetWeights({7.0, 1.0, 22.0});
    Cut right_bestCut = right_node.CalculateBestCut(CDFs);
    EXPECT_EQ( right_bestCut.feature, 1u );
    EXPECT_EQ( right_bestCut.index, 2u );
    EXPECT_DOUBLE_EQ( right_bestCut.gain, 0.375);
    EXPECT_TRUE( right_bestCut.valid );
    
    Node left_node(1,1);
    left_node.SetWeights({3.0, 9.0, 38.0});
    Cut left_bestCut = left_node.CalculateBestCut(CDFs);
    EXPECT_EQ( left_bestCut.feature, 1u );
    EXPECT_EQ( left_bestCut.index, 2u );
    EXPECT_DOUBLE_EQ( left_bestCut.gain, 0.53571428571428581);
    EXPECT_TRUE( left_bestCut.valid );

}

class TreeBuilderTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            eventSample = new EventSample(8, 2, {1, 1});
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 2 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 1 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 2 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 2 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 2 }), 1.0, false);

            auto &weights = eventSample->GetWeights();
            weights.Set(0, 4.0);
            weights.Set(1, 1.0);
            weights.Set(2, 2.0);
            weights.Set(3, 3.0);
            weights.Set(4, 2.0);
            weights.Set(5, 1.0);
            weights.Set(6, 3.0);
            weights.Set(7, 4.0);
        }

        virtual void TearDown() {
            delete eventSample;
        }

        EventSample *eventSample;
};

TEST_F(TreeBuilderTest, DeterminedCutsAreCorrect) {
    
    TreeBuilder dt(2, *eventSample);
    const auto &cuts = dt.GetCuts();
    EXPECT_EQ( cuts[0].feature, 0u );
    EXPECT_EQ( cuts[0].index, 2u );
    EXPECT_DOUBLE_EQ( cuts[0].gain, 1.875 );
    EXPECT_TRUE( cuts[0].valid );
    
    EXPECT_EQ( cuts[1].feature, 1u );
    EXPECT_EQ( cuts[1].index, 2u );
    EXPECT_DOUBLE_EQ( cuts[1].gain, 0.375 );
    EXPECT_TRUE( cuts[1].valid );
    
    EXPECT_EQ( cuts[2].feature, 1u );
    EXPECT_EQ( cuts[2].index, 2u );
    EXPECT_DOUBLE_EQ( cuts[2].gain, 0.53571428571428581 );
    EXPECT_TRUE( cuts[2].valid );

}

TEST_F(TreeBuilderTest, FlagsAreCorrectAfterTraining) {
    
    TreeBuilder dt(2, *eventSample);
    auto &flags = eventSample->GetFlags();
    EXPECT_EQ( flags.Get(0), 4 );
    EXPECT_EQ( flags.Get(1), 5 );
    EXPECT_EQ( flags.Get(2), 4 );
    EXPECT_EQ( flags.Get(3), 6 );
    EXPECT_EQ( flags.Get(4), 7 );
    EXPECT_EQ( flags.Get(5), 5 );
    EXPECT_EQ( flags.Get(6), 7 );
    EXPECT_EQ( flags.Get(7), 6 );

}


TEST_F(TreeBuilderTest, PuritiesOfNodesAreCorrectAfterTraining) {
    
    TreeBuilder dt(2, *eventSample);
    const auto &purities = dt.GetPurities();
    EXPECT_DOUBLE_EQ( purities[0], 0.5 );
    EXPECT_DOUBLE_EQ( purities[1], 0.875 );
    EXPECT_DOUBLE_EQ( purities[2], 0.25 );
    EXPECT_DOUBLE_EQ( purities[3], 1.0 );
    EXPECT_DOUBLE_EQ( purities[4], 0.5 );
    EXPECT_DOUBLE_EQ( purities[5], 0.4285714328289032 );
    EXPECT_DOUBLE_EQ( purities[6], 0.0 );

}

TEST_F(TreeBuilderTest, BoostWeightsOfNodesAreCorrectAfterTraining) {
    
    TreeBuilder dt(2, *eventSample);
    const auto &boostWeights = dt.GetBoostWeights();
    EXPECT_DOUBLE_EQ( boostWeights[0], 0.0 );
    EXPECT_DOUBLE_EQ( boostWeights[1], -1.0 );
    EXPECT_DOUBLE_EQ( boostWeights[2], 0.4285714328289032 );
    EXPECT_DOUBLE_EQ( boostWeights[3], -0.75 );
    EXPECT_DOUBLE_EQ( boostWeights[4], 0 );
    EXPECT_DOUBLE_EQ( boostWeights[5], 0.090909093618392944 );
    EXPECT_DOUBLE_EQ( boostWeights[6], 1.6666666269302368 );

}


class TreeTest : public ::testing::Test {
    protected:
        virtual void SetUp() {

            Cut cut1, cut2, cut3;
            cut1.feature = 0;
            cut1.index = 5;
            cut1.valid = true;
            cut2.feature = 1;
            cut2.index = 9;
            cut2.valid = true;
            cut3.valid = false;
            
            std::vector<Cut> cuts = {cut1, cut2, cut3};
            std::vector<float> purities = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7 };
            std::vector<float> boostWeights = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
            tree = new Tree(cuts, purities, boostWeights);            
        }

        virtual void TearDown() {
            delete tree;
        }

        Tree *tree;

};

TEST_F(TreeTest, ValueToNode) {

    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({2,3,31}) ), 3u );
    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({2,9,4}) ), 4u );
    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({4,9,31}) ), 4u );
    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({4,8,4}) ), 3u );
    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({5,8,31}) ), 2u );
    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({5,9,4}) ), 2u );

}

TEST_F(TreeTest, NaNToNode) {

    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({0,3,31}) ), 0u );
    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({2,3,0}) ), 3u );
    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({2,0,4}) ), 1u );
    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({2,9,4}) ), 4u );
    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({5,0,31}) ), 2u );
    EXPECT_EQ(tree->ValueToNode( std::vector<unsigned int>({5,9,0}) ), 2u );

}


TEST_F(TreeTest, Purities) {
    for(unsigned int i = 0; i < 7; ++i) {
        EXPECT_FLOAT_EQ(tree->GetPurity(i), 0.1*(i+1) );
    }
}

TEST_F(TreeTest, BoostWeights) {
    for(unsigned int i = 0; i < 7; ++i) {
        EXPECT_FLOAT_EQ(tree->GetBoostWeight(i), 1.0*(i+1) );
    }
}

class ForestBuilderTest : public ::testing::Test {
    protected:
        virtual void SetUp() {
            eventSample = new EventSample(20, 2, {1, 1});
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 2 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 1 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 1 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 1 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 1 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 2 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 2 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 2 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 1, 2 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 1 }), 1.0, true);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 2 }), 1.0, false);
            eventSample->AddEvent( std::vector<unsigned int>({ 2, 2 }), 1.0, false);
        }

        virtual void TearDown() {
            delete eventSample;
        }

        EventSample *eventSample;
};


TEST_F(ForestBuilderTest, F0AndShrinkageIsCorrect) {

    // Train without randomness and only with one layer per tree
    ForestBuilder forest(*eventSample, 0, 0.1, 1.0, 1); 
    EXPECT_FLOAT_EQ(forest.GetF0(), 0);
    EXPECT_FLOAT_EQ(forest.GetShrinkage(), 0.1);

} 

TEST_F(ForestBuilderTest, ForestIsCorrect) {

    // Train without randomness and only with one layer per tree
    ForestBuilder forest(*eventSample, 5, 0.1, 1.0, 1); 
    auto trees = forest.GetForest();
    EXPECT_EQ(trees[0].GetCut(0).feature, 0u);
    EXPECT_EQ(trees[1].GetCut(0).feature, 0u);
    EXPECT_EQ(trees[2].GetCut(0).feature, 1u);
    EXPECT_EQ(trees[3].GetCut(0).feature, 0u);
    EXPECT_EQ(trees[4].GetCut(0).feature, 1u);

}

class ForestTest : public ::testing::Test {
    protected:
        virtual void SetUp() {

            Cut cut1, cut2, cut3;
            cut1.feature = 0;
            cut1.index = 5;
            cut1.valid = true;
            cut1.gain = 2.0;
            cut2.feature = 1;
            cut2.index = 9;
            cut2.valid = true;
            cut2.gain = 1.0;
            cut3.valid = false;
            
            std::vector<Cut> cuts = {cut1, cut2, cut3};
            std::vector<float> purities = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7 };
            std::vector<float> boostWeights = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
            tree = new Tree(cuts, purities, boostWeights);            

            forest = new Forest(0.1, 1.0);
        }

        virtual void TearDown() {
            delete tree;
            delete forest;
        }

        Tree *tree;
        Forest *forest;

};

TEST_F(ForestTest, GetF) {

    std::vector<unsigned int> values = {1,1};
    EXPECT_FLOAT_EQ(forest->GetF(values), 1.0);
    forest->AddTree(*tree);
    EXPECT_FLOAT_EQ(forest->GetF(values), 1.4);
    forest->AddTree(*tree);
    EXPECT_FLOAT_EQ(forest->GetF(values), 1.8);

}

TEST_F(ForestTest, VariableRankingIsCorrect) {

    // Train without randomness and only with one layer per tree
    forest->AddTree(*tree);
    auto map = forest->GetVariableRanking();
    EXPECT_FLOAT_EQ(map[0], 2.0);
    EXPECT_FLOAT_EQ(map[1], 2.0);

} 

class IOTest : public ::testing::Test {
    protected:
        virtual void SetUp() {}
        virtual void TearDown() {}

};

TEST_F(IOTest, IOVector) {

    std::vector<double> before = {0.0, 1.0, 2.5, 3.2, -1.4, 0.0};

    std::stringstream stream;
    stream << before;

    std::vector<double> after;
    stream >> after;

    EXPECT_EQ(before.size(), after.size());
    for(unsigned int i = 0; i < before.size() and i < after.size(); ++i)
        EXPECT_DOUBLE_EQ(before[i], after[i]);

}

TEST_F(IOTest, IOFeatureBinning) {

    std::vector<double> binning = { 1.0f, 7.0f, 4.0f, 10.0f, 12.0f }; 
    FeatureBinning<double> before(2, binning.begin(), binning.end());
    const auto &before_binning = before.GetBinning();

    std::stringstream stream;
    stream << before;

    auto after = readFeatureBinningFromStream<double>(stream);
    const auto &after_binning = after.GetBinning();

    EXPECT_EQ(before.GetNLevels(), after.GetNLevels());
    EXPECT_EQ(before_binning.size(), after_binning.size());
    for(unsigned int i = 0; i < before_binning.size() and i < after_binning.size(); ++i)
        EXPECT_DOUBLE_EQ(before_binning[i], after_binning[i]);

}

TEST_F(IOTest, IOFeatureBinningVector) {

    std::vector<double> binning1 = { 1.0f, 7.0f, 4.0f, 10.0f, 12.0f }; 
    std::vector<double> binning2 = { 6.0f, 7.0f, 2.0f, 12.0f, 12.0f }; 
    std::vector<FeatureBinning<double>> before = {FeatureBinning<double>(2, binning1.begin(), binning1.end()),
                                                  FeatureBinning<double>(2, binning2.begin(), binning2.end())};

    std::stringstream stream;
    stream << before;

    std::vector<FeatureBinning<double>> after;
    stream >> after;

    EXPECT_EQ(before.size(), after.size());
    for(unsigned int j = 0; j < before.size() and j < after.size(); ++j) {

        auto &before_featureBinning = before[j];
        auto &after_featureBinning = after[j];
        const auto &after_binning = after_featureBinning.GetBinning();
        const auto &before_binning = before_featureBinning.GetBinning();

        EXPECT_EQ(before_featureBinning.GetNLevels(), after_featureBinning.GetNLevels());
        EXPECT_EQ(before_binning.size(), after_binning.size());
        for(unsigned int i = 0; i < before_binning.size() and i < after_binning.size(); ++i)
            EXPECT_DOUBLE_EQ(before_binning[i], after_binning[i]);

    }

}

TEST_F(IOTest, IOCut) {

    Cut before;
    before.feature = 1;
    before.gain = 3.4;
    before.index = 5;
    before.valid = true;

    std::stringstream stream;
    stream << before;

    Cut after;
    stream >> after;

    EXPECT_EQ(before.feature, after.feature);
    EXPECT_EQ(before.gain, after.gain);
    EXPECT_EQ(before.index, after.index);
    EXPECT_EQ(before.valid, after.valid);

}
            
TEST_F(IOTest, IOTree) {

    Cut cut1, cut2, cut3;
    cut1.feature = 0;
    cut1.index = 5;
    cut1.valid = true;
    cut1.gain = -3.0;
    cut2.feature = 1;
    cut2.index = 9;
    cut2.gain = 1.0;
    cut2.valid = true;
    cut3.feature = 0;
    cut3.index = 1;
    cut3.gain = 0.0;
    cut3.valid = false;
    
    std::vector<Cut> before_cuts = {cut1, cut2, cut3};
    std::vector<float> before_purities = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7 };
    std::vector<float> before_boostWeights = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
    Tree before(before_cuts, before_purities, before_boostWeights);            
    
    std::stringstream stream;
    stream << before;

    auto after = readTreeFromStream(stream);
    const auto &after_cuts = after.GetCuts();
    const auto &after_purities = after.GetPurities();
    const auto &after_boostWeights = after.GetBoostWeights();

    EXPECT_EQ(before_cuts.size(), after_cuts.size());
    for(unsigned int i = 0; i < before_cuts.size() and i < after_cuts.size(); ++i) {
        EXPECT_DOUBLE_EQ(before_cuts[i].feature, after_cuts[i].feature);
        EXPECT_DOUBLE_EQ(before_cuts[i].valid, after_cuts[i].valid);
        EXPECT_DOUBLE_EQ(before_cuts[i].index, after_cuts[i].index);
        EXPECT_DOUBLE_EQ(before_cuts[i].gain, after_cuts[i].gain);
    }
    
    EXPECT_EQ(before_purities.size(), after_purities.size());
    for(unsigned int i = 0; i < before_purities.size() and i < after_purities.size(); ++i)
        EXPECT_DOUBLE_EQ(before_purities[i], after_purities[i]);
    
    EXPECT_EQ(before_boostWeights.size(), after_boostWeights.size());
    for(unsigned int i = 0; i < before_boostWeights.size() and i < after_boostWeights.size(); ++i)
        EXPECT_DOUBLE_EQ(before_boostWeights[i], after_boostWeights[i]);

}

TEST_F(IOTest, IOForest) {

    Cut cut1, cut2, cut3, cut4;
    cut1.feature = 0;
    cut1.index = 5;
    cut1.valid = true;
    cut1.gain = -3.0;
    cut2.feature = 1;
    cut2.index = 9;
    cut2.gain = 1.0;
    cut2.valid = true;
    cut3.feature = 0;
    cut3.index = 1;
    cut3.gain = 0.0;
    cut3.valid = false;
    cut4.feature = 2;
    cut4.index = 3;
    cut4.valid = true;
    cut4.gain = 1.61;
    
    Forest before(0.5, 1.6);
    before.AddTree(Tree({cut1, cut2, cut3}, { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7 }, { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0}));
    before.AddTree(Tree({cut1, cut4, cut3}, { 0.6, 0.2, 0.5, 0.4, 0.5, 0.6, 0.7 }, { 2.0, 2.0, 3.0, 5.0, 5.0, 6.0, 1.0}));
    const auto &before_forest = before.GetForest();

    std::stringstream stream;
    stream << before;

    auto after = readForestFromStream(stream);
    const auto &after_forest = after.GetForest();

    EXPECT_EQ(before.GetF0(), after.GetF0());
    EXPECT_EQ(before.GetShrinkage(), after.GetShrinkage());

    EXPECT_EQ(before_forest.size(), after_forest.size());
    for(unsigned int j = 0; j < before_forest.size() and j < after_forest.size(); ++j) {

        auto &before_tree = before_forest[j];
        const auto &before_cuts = before_tree.GetCuts();
        const auto &before_purities = before_tree.GetPurities();
        const auto &before_boostWeights = before_tree.GetBoostWeights();
        
        auto &after_tree = after_forest[j];
        const auto &after_cuts = after_tree.GetCuts();
        const auto &after_purities = after_tree.GetPurities();
        const auto &after_boostWeights = after_tree.GetBoostWeights();

        EXPECT_EQ(before_cuts.size(), after_cuts.size());
        for(unsigned int i = 0; i < before_cuts.size() and i < after_cuts.size(); ++i) {
            EXPECT_DOUBLE_EQ(before_cuts[i].feature, after_cuts[i].feature);
            EXPECT_DOUBLE_EQ(before_cuts[i].valid, after_cuts[i].valid);
            EXPECT_DOUBLE_EQ(before_cuts[i].index, after_cuts[i].index);
            EXPECT_DOUBLE_EQ(before_cuts[i].gain, after_cuts[i].gain);
        }
        
        EXPECT_EQ(before_purities.size(), after_purities.size());
        for(unsigned int i = 0; i < before_purities.size() and i < after_purities.size(); ++i)
            EXPECT_DOUBLE_EQ(before_purities[i], after_purities[i]);
        
        EXPECT_EQ(before_boostWeights.size(), after_boostWeights.size());
        for(unsigned int i = 0; i < before_boostWeights.size() and i < after_boostWeights.size(); ++i)
            EXPECT_DOUBLE_EQ(before_boostWeights[i], after_boostWeights[i]);
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
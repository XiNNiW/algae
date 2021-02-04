#pragma once
#include <vector>
#include <memory>
#include <iostream>

namespace algae::dsp::shell{

    template<typename sample_t, typename frequency_t>
    struct dsp_node;

    template<typename sample_t, typename frequency_t>
    struct inlet{
        std::shared_ptr <dsp_node<sample_t,frequency_t>> connectedNode;
        size_t index;
        const sample_t getValue(){
            std::cout << "getting inlet value...\n";

            if(connectedNode){
                std::cout << "requested outlet is..."<<index<<"\n";
                std::cout << "connected node value is..."<<connectedNode.get()->getOutputValue(index)<<"\n";

            }

            return connectedNode?connectedNode.get()->getOutputValue(index):0;
        }
        void updateConnectedNode(const frequency_t& sampleRate=44100.0){
            if(connectedNode){
                std::cout << "updating connected node...\n";
                connectedNode.get()->update(sampleRate);
            }
        }
    };

    // template<typename sample_t, typename frequency_t>
    // struct AbstractDSPNode{
    //     virtual void update(std::vector<inlet<sample_t,frequency_t>>& inlets, std::vector<sample_t>& outlets, const frequency_t& sampleRate=44100.0)=0;
    //     virtual std::unique_ptr<AbstractDSPNode> clone() const = 0;
    // };

    // template<typename sample_t, typename frequency_t>
    // struct NullNode:AbstractDSPNode<sample_t,frequency_t>{
    //     virtual void update(std::vector<inlet<sample_t,frequency_t>>& inlets, std::vector<sample_t>& outlets, const frequency_t& sampleRate=44100.0){};
    //     virtual std::unique_ptr<AbstractDSPNode<sample_t,frequency_t>> clone() const{
    //         return std::make_unique<AbstractDSPNode<sample_t,frequency_t>>(*this);
    //     }
    // };

    template<typename sample_t, typename frequency_t>
    std::vector<inlet<sample_t,frequency_t>> make_inlets(const int& number_of_inlets){
        auto inlets = std::vector<inlet<sample_t,frequency_t>>();
        for(int idx=0;idx<number_of_inlets; idx++){
            inlets.push_back(inlet<sample_t,frequency_t>());
        }
        return inlets;
    }

    template<typename sample_t, typename frequency_t>
    std::vector<sample_t> make_outlets(const int& number_of_outlets){
        auto outlets = std::vector<sample_t>();
        for(int idx=0;idx<number_of_outlets; idx++){
            outlets.push_back(0);
        }
        return outlets;
    }

    template<typename sample_t, typename frequency_t>
    struct dsp_node{
        std::vector<inlet<sample_t,frequency_t>> inlets;
        std::vector<sample_t> outlets;

        dsp_node(
            std::vector<inlet<sample_t,frequency_t>> inlets,
            std::vector<sample_t> outlets
        )
        :inlets(inlets)
        ,outlets(outlets)
        {}

        virtual void process(const frequency_t& sampleRate=44100.0)=0;
        virtual std::shared_ptr<dsp_node<sample_t,frequency_t>> getSharedPtr()=0;
        virtual std::unique_ptr<dsp_node<sample_t,frequency_t>> getUniquePtr()=0;
        virtual void update(const frequency_t& sampleRate=44100.0){
            std::cout<<"in parent update... updatind deps\n";
            for(auto inlet: inlets){
                inlet.updateConnectedNode(sampleRate);
            }

            this->process(sampleRate);
        };
        const sample_t getOutputValue(size_t indexOfOutlet){
            return outlets.size()>indexOfOutlet? outlets[indexOfOutlet] : 0;
        }

        virtual ~dsp_node()=default;

    };



    template<typename sample_t, typename frequency_t>
    void connect(dsp_node<sample_t, frequency_t>& fromNode, int outletNumber, dsp_node<sample_t, frequency_t>& toNode, int inletNumber){
        auto& input = toNode.inlets[inletNumber];
        input.connectedNode = fromNode.getSharedPtr();
        input.index = outletNumber;
    }

    template<typename sample_t, typename frequency_t>
    void connect(dsp_node<sample_t, frequency_t>* fromNode, int outletNumber, dsp_node<sample_t, frequency_t>* toNode, int inletNumber){
        auto& input = toNode->inlets[inletNumber];
        input.connectedNode = fromNode->getSharedPtr();
        input.index = outletNumber;
    }

    template<typename sample_t, typename frequency_t>
    void connect(dsp_node<sample_t, frequency_t>* fromNode, int outletNumber, dsp_node<sample_t, frequency_t>& toNode, int inletNumber){
        auto& input = toNode.inlets[inletNumber];
        input.connectedNode = fromNode->getSharedPtr();
        input.index = outletNumber;
    }



}
